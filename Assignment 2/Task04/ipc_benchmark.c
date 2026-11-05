#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define CHUNK_SIZE (256U * 1024U)
#define TRIALS 3
#define MIB (1024U * 1024U)

typedef struct {
    sem_t empty;
    sem_t full;
    size_t length;
    int finished;
    uint64_t checksum;
    unsigned char data[CHUNK_SIZE];
} SharedBuffer;

static int sem_wait_retry(sem_t *sem)
{
    while (sem_wait(sem) == -1) {
        if (errno == EINTR) continue;
        return -1;
    }
    return 0;
}

static int write_all(int fd, const void *buffer, size_t length)
{
    const unsigned char *p = (const unsigned char *)buffer;
    size_t done = 0;

    while (done < length) {
        ssize_t n = write(fd, p + done, length - done);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) return -1;
        done += (size_t)n;
    }
    return 0;
}

static int read_exact(int fd, void *buffer, size_t length)
{
    unsigned char *p = (unsigned char *)buffer;
    size_t done = 0;

    while (done < length) {
        ssize_t n = read(fd, p + done, length - done);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) return -1;
        done += (size_t)n;
    }
    return 0;
}

static double monotonic_seconds(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

/* Repeatable byte pattern shared by both implementations. */
static unsigned char payload_byte(size_t offset)
{
    return (unsigned char)(((offset * 31U) + 7U) & 0xffU);
}

static void fill_payload(unsigned char *buffer, size_t offset, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = payload_byte(offset + i);
    }
}

static uint64_t expected_checksum(size_t total_bytes)
{
    uint64_t sum = 0;
    size_t cycles = total_bytes / 256U;
    size_t remainder = total_bytes % 256U;

    /* The pattern repeats every 256 bytes. */
    sum += (uint64_t)cycles * UINT64_C(32640);
    for (size_t i = 0; i < remainder; ++i) {
        sum += payload_byte(i);
    }
    return sum;
}

static int wait_for_child(pid_t pid)
{
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "IPC consumer child failed.\n");
        return -1;
    }
    return 0;
}

/* Pipe implementation: parent writes bytes; child reads until EOF. */
static int pipe_transfer(size_t total_bytes, double *elapsed_ms)
{
    int data_pipe[2], result_pipe[2];
    if (pipe(data_pipe) == -1 || pipe(result_pipe) == -1) {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(data_pipe[0]); close(data_pipe[1]);
        close(result_pipe[0]); close(result_pipe[1]);
        return -1;
    }

    if (pid == 0) {
        close(data_pipe[1]);
        close(result_pipe[0]);

        unsigned char buffer[CHUNK_SIZE];
        uint64_t checksum = 0;
        for (;;) {
            ssize_t n = read(data_pipe[0], buffer, sizeof(buffer));
            if (n < 0) {
                if (errno == EINTR) continue;
                perror("pipe consumer read");
                _exit(2);
            }
            if (n == 0) break; /* EOF: parent closed its write end. */

            for (ssize_t i = 0; i < n; ++i) {
                checksum += buffer[i];
            }
        }

        close(data_pipe[0]);
        if (write_all(result_pipe[1], &checksum, sizeof(checksum)) != 0) {
            perror("pipe result write");
            _exit(3);
        }
        close(result_pipe[1]);
        _exit(0);
    }

    close(data_pipe[0]);
    close(result_pipe[1]);

    unsigned char buffer[CHUNK_SIZE];
    uint64_t child_checksum = 0;
    size_t offset = 0;
    double start = monotonic_seconds();

    while (offset < total_bytes) {
        size_t amount = total_bytes - offset;
        if (amount > sizeof(buffer)) amount = sizeof(buffer);
        fill_payload(buffer, offset, amount);

        if (write_all(data_pipe[1], buffer, amount) != 0) {
            perror("pipe producer write");
            close(data_pipe[1]);
            close(result_pipe[0]);
            (void)waitpid(pid, NULL, 0);
            return -1;
        }
        offset += amount;
    }

    close(data_pipe[1]); /* EOF tells the child that the stream is complete. */

    if (read_exact(result_pipe[0], &child_checksum,
                   sizeof(child_checksum)) != 0) {
        fprintf(stderr, "Could not receive pipe checksum.\n");
        close(result_pipe[0]);
        (void)waitpid(pid, NULL, 0);
        return -1;
    }
    close(result_pipe[0]);

    if (wait_for_child(pid) != 0) return -1;

    *elapsed_ms = (monotonic_seconds() - start) * 1000.0;
    if (child_checksum != expected_checksum(total_bytes)) {
        fprintf(stderr, "Pipe checksum mismatch.\n");
        return -1;
    }
    return 0;
}

/* Shared-memory implementation: a single-slot shared buffer synchronized
   between parent producer and child consumer using process-shared semaphores. */
static int shared_memory_transfer(size_t total_bytes, double *elapsed_ms)
{
    char shm_name[80];
    snprintf(shm_name, sizeof(shm_name), "/task4_ipc_%ld_%zu",
             (long)getpid(), total_bytes);

    int fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(fd, (off_t)sizeof(SharedBuffer)) == -1) {
        perror("ftruncate");
        close(fd); shm_unlink(shm_name);
        return -1;
    }

    SharedBuffer *shared = mmap(NULL, sizeof(*shared),
                                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        close(fd); shm_unlink(shm_name);
        return -1;
    }

    memset(shared, 0, sizeof(*shared));
    if (sem_init(&shared->empty, 1, 1) == -1 ||
        sem_init(&shared->full, 1, 0) == -1) {
        perror("sem_init");
        munmap(shared, sizeof(*shared));
        close(fd); shm_unlink(shm_name);
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        sem_destroy(&shared->empty);
        sem_destroy(&shared->full);
        munmap(shared, sizeof(*shared));
        close(fd); shm_unlink(shm_name);
        return -1;
    }

    if (pid == 0) {
        uint64_t checksum = 0;

        for (;;) {
            if (sem_wait_retry(&shared->full) == -1) {
                perror("consumer sem_wait");
                _exit(2);
            }

            if (shared->finished) break;

            for (size_t i = 0; i < shared->length; ++i) {
                checksum += shared->data[i];
            }

            if (sem_post(&shared->empty) == -1) {
                perror("consumer sem_post");
                _exit(3);
            }
        }

        shared->checksum = checksum;
        _exit(0);
    }

    size_t offset = 0;
    double start = monotonic_seconds();

    while (offset < total_bytes) {
        size_t amount = total_bytes - offset;
        if (amount > sizeof(shared->data)) amount = sizeof(shared->data);

        if (sem_wait_retry(&shared->empty) == -1) {
            perror("producer sem_wait");
            (void)waitpid(pid, NULL, 0);
            return -1;
        }

        fill_payload(shared->data, offset, amount);
        shared->length = amount;
        shared->finished = 0;

        if (sem_post(&shared->full) == -1) {
            perror("producer sem_post");
            (void)waitpid(pid, NULL, 0);
            return -1;
        }

        offset += amount;
    }

    /* Send an explicit end-of-stream marker. */
    if (sem_wait_retry(&shared->empty) == -1) {
        perror("producer final sem_wait");
        (void)waitpid(pid, NULL, 0);
        return -1;
    }
    shared->length = 0;
    shared->finished = 1;
    if (sem_post(&shared->full) == -1) {
        perror("producer final sem_post");
        (void)waitpid(pid, NULL, 0);
        return -1;
    }

    if (wait_for_child(pid) != 0) {
        sem_destroy(&shared->empty);
        sem_destroy(&shared->full);
        munmap(shared, sizeof(*shared));
        close(fd); shm_unlink(shm_name);
        return -1;
    }

    *elapsed_ms = (monotonic_seconds() - start) * 1000.0;

    int valid = (shared->checksum == expected_checksum(total_bytes));
    if (!valid) fprintf(stderr, "Shared-memory checksum mismatch.\n");

    sem_destroy(&shared->empty);
    sem_destroy(&shared->full);
    munmap(shared, sizeof(*shared));
    close(fd);
    shm_unlink(shm_name);

    return valid ? 0 : -1;
}

static double median3(double a[TRIALS])
{
    double v[TRIALS];
    memcpy(v, a, sizeof(v));
    for (int i = 0; i < TRIALS - 1; ++i) {
        for (int j = i + 1; j < TRIALS; ++j) {
            if (v[j] < v[i]) {
                double tmp = v[i]; v[i] = v[j]; v[j] = tmp;
            }
        }
    }
    return v[TRIALS / 2];
}

int main(void)
{
    const int sizes_mb[] = {1, 10, 100};

    printf("IPC transfer benchmark: pipe vs shared memory + semaphores\n");
    printf("Buffer chunk: %u bytes; trials per case: %d\n", CHUNK_SIZE, TRIALS);
    printf("Sizes use MiB (1 MiB = 1,048,576 bytes).\n");
    printf("Timing covers data transfer and consumer completion; setup is outside the timed interval.\n\n");

    printf("%-10s %-18s %14s %s\n",
           "Size", "Method", "Median ms", "Validation");

    for (size_t s = 0; s < sizeof(sizes_mb) / sizeof(sizes_mb[0]); ++s) {
        size_t bytes = (size_t)sizes_mb[s] * MIB;
        double pipe_times[TRIALS], shm_times[TRIALS];

        for (int t = 0; t < TRIALS; ++t) {
            if (pipe_transfer(bytes, &pipe_times[t]) != 0) {
                fprintf(stderr, "Pipe transfer failed for %d MiB.\n", sizes_mb[s]);
                return EXIT_FAILURE;
            }
        }

        for (int t = 0; t < TRIALS; ++t) {
            if (shared_memory_transfer(bytes, &shm_times[t]) != 0) {
                fprintf(stderr, "Shared-memory transfer failed for %d MiB.\n", sizes_mb[s]);
                return EXIT_FAILURE;
            }
        }

        printf("%-10d %-18s %14.3f PASS\n",
               sizes_mb[s], "Pipe", median3(pipe_times));
        printf("%-10d %-18s %14.3f PASS\n",
               sizes_mb[s], "Shared memory", median3(shm_times));
    }

    printf("\nAll transfers were checksum-validated. Use these timings from your own machine in the report.\n");
    return EXIT_SUCCESS;
}
