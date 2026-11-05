#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAXF 10
#define FIXED_LEN 20
#define BELADY_LEN 12
#define BENCH_LEN 100000
#define PAGE_DOMAIN 256
#define TRIALS 3

static const int fixed_ref[FIXED_LEN] = {
    7, 0, 1, 2, 0, 3, 0, 4, 2, 3,
    0, 3, 2, 1, 2, 0, 1, 7, 0, 1
};

static const int belady_ref[BELADY_LEN] = {
    1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
};

static int find(const int f[], int n, int page)
{
    for (int i = 0; i < n; i++)
        if (f[i] == page)
            return i;
    return -1;
}

static int fifo(const int ref[], int len, int frames)
{
    int f[MAXF], used = 0, next = 0, faults = 0;

    for (int i = 0; i < len; i++) {
        if (find(f, used, ref[i]) >= 0)
            continue;

        faults++;

        if (used < frames)
            f[used++] = ref[i];
        else {
            f[next] = ref[i];
            next = (next + 1) % frames;
        }
    }

    return faults;
}

/* Complete TODO 1: exact LRU replacement. */
static int lru(const int ref[], int len, int frames)
{
    int f[MAXF], last[MAXF], used = 0, faults = 0;

    for (int i = 0; i < len; i++) {
        int k = find(f, used, ref[i]);

        if (k >= 0) {
            last[k] = i;
            continue;
        }

        faults++;

        int victim;
        if (used < frames) {
            victim = used++;
            f[victim] = ref[i];
        } else {
            victim = 0;
            for (int j = 1; j < frames; j++) {
                if (last[j] < last[victim])
                    victim = j;
            }
            f[victim] = ref[i];
        }

        last[victim] = i;
    }

    return faults;
}

/* Simple forward-looking Optimal implementation for short reference strings. */
static int optimal_simple(const int ref[], int len, int frames)
{
    int f[MAXF], used = 0, faults = 0;

    for (int i = 0; i < len; i++) {
        if (find(f, used, ref[i]) >= 0)
            continue;

        faults++;

        if (used < frames) {
            f[used++] = ref[i];
            continue;
        }

        int victim = 0;
        int farthest = -1;

        for (int j = 0; j < frames; j++) {
            int next = len; /* page is never used again */

            for (int t = i + 1; t < len; t++) {
                if (ref[t] == f[j]) {
                    next = t;
                    break;
                }
            }

            if (next > farthest) {
                farthest = next;
                victim = j;
            }
        }

        f[victim] = ref[i];
    }

    return faults;
}

/* Precompute each reference's next occurrence for efficient Optimal timing.
   Pages must be in [0, PAGE_DOMAIN). */
static int optimal_fast(const int ref[], int len, int frames)
{
    int next_occurrence[BENCH_LEN];
    int next_for_page[PAGE_DOMAIN];
    int f[MAXF], used = 0, faults = 0;

    for (int p = 0; p < PAGE_DOMAIN; p++)
        next_for_page[p] = len;

    for (int i = len - 1; i >= 0; i--) {
        int p = ref[i];
        next_occurrence[i] = next_for_page[p];
        next_for_page[p] = i;
    }

    for (int p = 0; p < PAGE_DOMAIN; p++)
        next_for_page[p] = len;

    for (int i = 0; i < len; i++) {
        int page = ref[i];
        int k = find(f, used, page);

        if (k >= 0) {
            next_for_page[page] = next_occurrence[i];
            continue;
        }

        faults++;

        if (used < frames) {
            f[used++] = page;
            next_for_page[page] = next_occurrence[i];
            continue;
        }

        int victim = 0;
        int farthest = -1;

        for (int j = 0; j < frames; j++) {
            int next = next_for_page[f[j]];
            if (next > farthest) {
                farthest = next;
                victim = j;
            }
        }

        f[victim] = page;
        next_for_page[page] = next_occurrence[i];
    }

    return faults;
}

/* Clock / second-chance replacement: one reference bit per frame. */
static int clock_second_chance(const int ref[], int len, int frames)
{
    int f[MAXF], used = 0, hand = 0, faults = 0;
    unsigned char reference_bit[MAXF] = {0};

    for (int i = 0; i < len; i++) {
        int k = find(f, used, ref[i]);

        if (k >= 0) {
            reference_bit[k] = 1;
            continue;
        }

        faults++;

        if (used < frames) {
            f[used] = ref[i];
            reference_bit[used] = 1;
            used++;
            hand = used % frames;
            continue;
        }

        while (reference_bit[hand] == 1) {
            reference_bit[hand] = 0;
            hand = (hand + 1) % frames;
        }

        f[hand] = ref[i];
        reference_bit[hand] = 1;
        hand = (hand + 1) % frames;
    }

    return faults;
}

static uint32_t rng_state = 0x5eed1234U;
static uint32_t next_random(void)
{
    rng_state = rng_state * 1664525U + 1013904223U;
    return rng_state;
}

/* Strong locality: 90% references to a hot set of 8 pages, 10% to 64 pages. */
static void generate_locality(int *ref, int len)
{
    for (int i = 0; i < len; i++) {
        uint32_t r = (next_random() >> 16) % 100U;
        if (r < 90U)
            ref[i] = (int)((next_random() >> 16) % 8U);
        else
            ref[i] = (int)((next_random() >> 16) % 64U);
    }
}

/* Random workload: uniform selection from 64 virtual pages. */
static void generate_random(int *ref, int len)
{
    for (int i = 0; i < len; i++)
        ref[i] = (int)((next_random() >> 16) % 64U);
}

static double now_seconds(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static double median3(double v[TRIALS])
{
    double a[TRIALS];
    memcpy(a, v, sizeof(a));

    for (int i = 0; i < TRIALS - 1; i++) {
        for (int j = i + 1; j < TRIALS; j++) {
            if (a[j] < a[i]) {
                double t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }

    return a[TRIALS / 2];
}

static void report_fixed_string(void)
{
    puts("=== Given 20-reference string ===");
    printf("Reference string:");
    for (int i = 0; i < FIXED_LEN; i++)
        printf(" %d", fixed_ref[i]);
    puts("");

    for (int frames = 3; frames <= 4; frames++) {
        printf("%d frames: FIFO %d, LRU %d, Optimal %d\n",
               frames,
               fifo(fixed_ref, FIXED_LEN, frames),
               lru(fixed_ref, FIXED_LEN, frames),
               optimal_simple(fixed_ref, FIXED_LEN, frames));
    }

    printf("Belady string, FIFO: 3 frames %d faults, 4 frames %d faults\n",
           fifo(belady_ref, BELADY_LEN, 3),
           fifo(belady_ref, BELADY_LEN, 4));
}

static void write_fault_csv(const char *filename, const int *ref,
                            int len, const char *label)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Workload,Frames,FIFO,LRU,Optimal,Clock\n");

    for (int frames = 3; frames <= 10; frames++) {
        fprintf(fp, "%s,%d,%d,%d,%d,%d\n",
                label, frames,
                fifo(ref, len, frames),
                lru(ref, len, frames),
                optimal_fast(ref, len, frames),
                clock_second_chance(ref, len, frames));
    }

    fclose(fp);
}

static void benchmark_100k(const int *locality, const int *random)
{
    FILE *fp = fopen("runtime_100k.csv", "w");
    if (fp == NULL) {
        perror("runtime_100k.csv");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "Workload,Algorithm,References,Frames,MedianMilliseconds,Faults\n");

    const char *workload_names[] = {"StrongLocality", "Random"};
    const int *refs[] = {locality, random};

    puts("=== Runtime benchmark: 100,000 references, median of 3 runs ===");
    puts("Workload          Algorithm  Frames  Median ms       Faults");

    for (int w = 0; w < 2; w++) {
        for (int alg = 0; alg < 4; alg++) {
            for (int frames = 3; frames <= 10; frames++) {
                double times[TRIALS];
                int faults = 0;

                for (int t = 0; t < TRIALS; t++) {
                    double start = now_seconds();

                    if (alg == 0)
                        faults = fifo(refs[w], BENCH_LEN, frames);
                    else if (alg == 1)
                        faults = lru(refs[w], BENCH_LEN, frames);
                    else if (alg == 2)
                        faults = optimal_fast(refs[w], BENCH_LEN, frames);
                    else
                        faults = clock_second_chance(refs[w], BENCH_LEN, frames);

                    times[t] = (now_seconds() - start) * 1000.0;
                }

                double med = median3(times);
                const char *alg_name[] = {"FIFO", "LRU", "Optimal", "Clock"};

                printf("%-17s %-9s %2d      %10.3f      %d\n",
                       workload_names[w], alg_name[alg], frames, med, faults);

                fprintf(fp, "%s,%s,%d,%d,%.6f,%d\n",
                        workload_names[w], alg_name[alg], BENCH_LEN,
                        frames, med, faults);
            }
        }
    }

    fclose(fp);
}

int main(void)
{
    int *locality = malloc(BENCH_LEN * sizeof(*locality));
    int *random = malloc(BENCH_LEN * sizeof(*random));

    if (locality == NULL || random == NULL) {
        perror("malloc reference strings");
        free(locality);
        free(random);
        return EXIT_FAILURE;
    }

    report_fixed_string();

    puts("\n=== Generate workloads and count faults for 3-10 frames ===");
    generate_locality(locality, BENCH_LEN);
    generate_random(random, BENCH_LEN);

    write_fault_csv("faults_locality.csv", locality, BENCH_LEN, "StrongLocality");
    write_fault_csv("faults_random.csv", random, BENCH_LEN, "Random");

    puts("Saved faults_locality.csv and faults_random.csv.");

    benchmark_100k(locality, random);

    free(locality);
    free(random);
    return EXIT_SUCCESS;
}
