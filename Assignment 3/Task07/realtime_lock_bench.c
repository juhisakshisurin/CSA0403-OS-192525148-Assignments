#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_MIB 4
#define PASSES 3

static uint64_t ns_now(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    return (uint64_t)ts.tv_sec * UINT64_C(1000000000)
         + (uint64_t)ts.tv_nsec;
}

static int run_case(size_t bytes, size_t page_size, int lock_memory,
                    const char *label)
{
    unsigned char *memory = mmap(NULL, bytes, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    if (lock_memory) {
        if (mlock(memory, bytes) != 0) {
            fprintf(stderr, "mlock(%zu bytes) failed: %s\n",
                    bytes, strerror(errno));
            munmap(memory, bytes);
            return 1; /* skipped, not an execution crash */
        }
    }

    size_t pages = (bytes + page_size - 1) / page_size;

    /* Prefault every page before measuring the locked control loop.
       For the unlocked case, this is repeated after MADV_DONTNEED. */
    if (lock_memory) {
        for (size_t i = 0; i < pages; ++i)
            memory[i * page_size] = (unsigned char)i;
    }

    struct rusage before, after;
    if (getrusage(RUSAGE_SELF, &before) != 0) {
        perror("getrusage before");
        if (lock_memory) munlock(memory, bytes);
        munmap(memory, bytes);
        return -1;
    }

    uint64_t total_ns = 0;
    uint64_t max_ns = 0;
    volatile unsigned char *v = memory;

    for (int pass = 0; pass < PASSES; ++pass) {
        if (!lock_memory) {
#ifdef MADV_DONTNEED
            if (madvise(memory, bytes, MADV_DONTNEED) != 0) {
                perror("madvise");
                munmap(memory, bytes);
                return -1;
            }
#endif
        }

        for (size_t i = 0; i < pages; ++i) {
            size_t offset = i * page_size;
            uint64_t start = ns_now();

            /* One representative page access by the time-critical loop. */
            v[offset] = (unsigned char)(v[offset] + 1U);

            uint64_t elapsed = ns_now() - start;
            total_ns += elapsed;
            if (elapsed > max_ns)
                max_ns = elapsed;
        }
    }

    if (getrusage(RUSAGE_SELF, &after) != 0) {
        perror("getrusage after");
        if (lock_memory) munlock(memory, bytes);
        munmap(memory, bytes);
        return -1;
    }

    long minor = after.ru_minflt - before.ru_minflt;
    long major = after.ru_majflt - before.ru_majflt;
    uint64_t accesses = (uint64_t)pages * PASSES;

    printf("%-8s bytes=%zu pages=%zu accesses=%" PRIu64
           " avg_touch=%.1f ns max_observed_touch=%" PRIu64
           " ns minor_faults=%ld major_faults=%ld\n",
           label, bytes, pages, accesses,
           (double)total_ns / (double)accesses,
           max_ns, minor, major);

    if (lock_memory && (minor != 0 || major != 0)) {
        puts("NOTE: Some faults were counted in the process during the measurement interval.");
    }

    if (lock_memory)
        (void)munlock(memory, bytes);

    munmap(memory, bytes);
    return 0;
}

int main(int argc, char **argv)
{
    size_t mib = DEFAULT_MIB;

    if (argc == 2) {
        char *end = NULL;
        unsigned long long value = strtoull(argv[1], &end, 10);

        if (end == argv[1] || *end != '\0' ||
            value == 0 || value > 64) {
            fprintf(stderr, "Usage: %s [working_set_MiB, 1..64]\n", argv[0]);
            return EXIT_FAILURE;
        }
        mib = (size_t)value;
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [working_set_MiB, 1..64]\n", argv[0]);
        return EXIT_FAILURE;
    }

    long native = sysconf(_SC_PAGESIZE);
    if (native <= 0) {
        fprintf(stderr, "Cannot determine native page size.\n");
        return EXIT_FAILURE;
    }

    size_t bytes = mib * 1024U * 1024U;
    size_t page_size = (size_t)native;

    printf("Control-loop memory test: %zu MiB, native page size=%zu bytes\n",
           mib, page_size);
    puts("Unlocked case discards pages before each pass to induce demand faults.");
    puts("Locked case calls mlock() and prefaults pages before measurement.");
    puts("Reported maximum is an observed per-page-touch latency, not a hard-real-time WCET guarantee.");

    if (run_case(bytes, page_size, 0, "UNLOCKED") != 0)
        return EXIT_FAILURE;

    int locked_result = run_case(bytes, page_size, 1, "LOCKED");

    if (locked_result == 1) {
        puts("LOCKED test SKIPPED because the process memlock limit/permission was insufficient.");
        puts("Check: ulimit -l");
        puts("Only increase the limit in a controlled test environment.");
    } else if (locked_result < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
