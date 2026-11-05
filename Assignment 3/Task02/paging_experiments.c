#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROCESS_COUNT 100
#define SAMPLE_SIZE_MAX (10U * 1024U * 1024U)

static uint32_t rng_state = 0x1234abcdU;

static uint32_t next_random(void)
{
    /* Deterministic LCG so results are reproducible across runs. */
    rng_state = rng_state * 1664525U + 1013904223U;
    return rng_state;
}

static size_t round_up_to_page(size_t size, size_t page_size)
{
    return ((size + page_size - 1) / page_size) * page_size;
}

static void fragmentation_experiment(size_t page_size,
                                     const size_t sizes[PROCESS_COUNT])
{
    uint64_t total_allocated = 0;
    uint64_t total_requested = 0;

    for (int i = 0; i < PROCESS_COUNT; ++i) {
        size_t allocated = round_up_to_page(sizes[i], page_size);
        total_allocated += allocated;
        total_requested += sizes[i];
    }

    uint64_t waste = total_allocated - total_requested;

    printf("%-12zu %-18" PRIu64 " %-18" PRIu64
           " %-18" PRIu64 " %.2f\n",
           page_size,
           total_requested,
           total_allocated,
           waste,
           (double)waste / PROCESS_COUNT);
}

int main(void)
{
    size_t sizes[PROCESS_COUNT];

    /* 100 deterministic pseudo-random process sizes from 1 byte to 10 MiB. */
    for (int i = 0; i < PROCESS_COUNT; ++i) {
        sizes[i] = 1U + (size_t)(next_random() % SAMPLE_SIZE_MAX);
    }

    puts("Internal fragmentation experiment: 100 processes");
    puts("Random-size range: 1 byte to 10 MiB; fixed seed for repeatability.");
    puts("Page size (B) Requested (B) Allocated (B) Wasted (B) Average waste (B)");
    fragmentation_experiment(1024, sizes);
    fragmentation_experiment(4096, sizes);
    fragmentation_experiment(2U * 1024U * 1024U, sizes);

    puts("\nTLB effective access time example:");
    puts("Assumptions: memory access = 100 ns; TLB lookup = 10 ns.");
    puts("No TLB: 2 memory accesses = 200 ns.");
    const double t = 10.0, m = 100.0;
    const double hit_ratios[] = {0.80, 0.95, 0.99};

    for (size_t i = 0; i < sizeof(hit_ratios) / sizeof(hit_ratios[0]); ++i) {
        double h = hit_ratios[i];
        double eat = h * (t + m) + (1.0 - h) * (t + 2.0 * m);
        printf("TLB hit ratio %.0f%%: EAT = %.2f ns\n", h * 100.0, eat);
    }

    return 0;
}
