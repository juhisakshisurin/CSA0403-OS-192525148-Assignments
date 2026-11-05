#define _POSIX_C_SOURCE 200809L
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

static double seconds_now(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static uint32_t rng_state = 0x76543210U;
static uint32_t next_random(void)
{
    rng_state = rng_state * 1664525U + 1013904223U;
    return rng_state;
}

static void shuffle(size_t *a, size_t n)
{
    for (size_t i = n; i > 1; --i) {
        size_t j = (size_t)(next_random() % (uint32_t)i);
        size_t tmp = a[i - 1];
        a[i - 1] = a[j];
        a[j] = tmp;
    }
}

static int run_order(size_t bytes, size_t page_size, int random_order,
                     double *elapsed_ms, long *minor, long *major,
                     uint64_t *checksum)
{
    unsigned char *memory = mmap(NULL, bytes, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

#ifdef MADV_NOHUGEPAGE
    /* Keep the experiment closer to base-page granularity where supported. */
    (void)madvise(memory, bytes, MADV_NOHUGEPAGE);
#endif

    size_t pages = (bytes + page_size - 1) / page_size;
    size_t *order = malloc(pages * sizeof(*order));
    if (order == NULL) {
        perror("malloc order");
        munmap(memory, bytes);
        return -1;
    }

    for (size_t i = 0; i < pages; ++i)
        order[i] = i;

    if (random_order)
        shuffle(order, pages);

    struct rusage before, after;
    if (getrusage(RUSAGE_SELF, &before) != 0) {
        perror("getrusage");
        free(order);
        munmap(memory, bytes);
        return -1;
    }

    volatile unsigned char *v = memory;
    uint64_t sum = 0;
    double start = seconds_now();

    /* Revisit the full working set several times. In a memory-limited VM,
       pages evicted to swap may have to be paged back during later passes. */
    const int passes = 3;
    for (int pass = 0; pass < passes; ++pass) {
        if (random_order) shuffle(order, pages);

        for (size_t i = 0; i < pages; ++i) {
            size_t offset = order[i] * page_size;
            unsigned char value =
                (unsigned char)((order[i] * 17U + (unsigned)pass + 13U) & 0xffU);
            v[offset] = value;
            sum += v[offset];
        }
    }

    *elapsed_ms = (seconds_now() - start) * 1000.0;

    if (getrusage(RUSAGE_SELF, &after) != 0) {
        perror("getrusage");
        free(order);
        munmap(memory, bytes);
        return -1;
    }

    *minor = after.ru_minflt - before.ru_minflt;
    *major = after.ru_majflt - before.ru_majflt;
    *checksum = sum;

    free(order);
    munmap(memory, bytes);
    return 0;
}

int main(int argc, char **argv)
{
    size_t mb = 64;

    if (argc == 2) {
        char *end = NULL;
        unsigned long long parsed = strtoull(argv[1], &end, 10);
        if (end == argv[1] || *end != '\0' || parsed == 0 ||
            parsed > (unsigned long long)(SIZE_MAX / (1024U * 1024U))) {
            fprintf(stderr, "Usage: %s [memory_size_MB]\n", argv[0]);
            return EXIT_FAILURE;
        }
        mb = (size_t)parsed;
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [memory_size_MB]\n", argv[0]);
        return EXIT_FAILURE;
    }

    long native_page = sysconf(_SC_PAGESIZE);
    if (native_page <= 0) {
        fprintf(stderr, "Could not obtain the native page size.\n");
        return EXIT_FAILURE;
    }

    size_t bytes = mb * 1024U * 1024U;
    size_t page_size = (size_t)native_page;
    size_t pages = (bytes + page_size - 1) / page_size;

    printf("Memory access experiment: %zu MiB, page size %zu bytes, "
           "%zu pages\n", mb, page_size, pages);
    puts("Each order writes one byte in every page of a fresh anonymous mapping.");
    puts("The entire working set is revisited for 3 passes.");

    const char *names[] = {"sequential", "random"};
    for (int mode = 0; mode < 2; ++mode) {
        double elapsed = 0.0;
        long minor = 0, major = 0;
        uint64_t checksum = 0;

        if (run_order(bytes, page_size, mode, &elapsed,
                      &minor, &major, &checksum) != 0)
            return EXIT_FAILURE;

        printf("%-10s time=%.3f ms minor_faults=%ld major_faults=%ld "
               "checksum=%" PRIu64 "\n",
               names[mode], elapsed, minor, major, checksum);
    }

    return EXIT_SUCCESS;
}
