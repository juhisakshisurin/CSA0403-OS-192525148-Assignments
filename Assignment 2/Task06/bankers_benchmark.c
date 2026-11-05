#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESOURCES 3
#define TRIALS 5
#define MAX_PROCESSES 1000

static int maximum[MAX_PROCESSES][RESOURCES];
static int allocation[MAX_PROCESSES][RESOURCES];
static int available[RESOURCES];
static int work[RESOURCES];
static int finish[MAX_PROCESSES];
static int sequence[MAX_PROCESSES];

static double monotonic_seconds(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

/* Construct a safe input whose scan order forces repeated passes:
   P0 needs n units of resource 0, P1 needs n-1, ..., P(n-1) needs 1.
   Each process holds one unit; available starts at one. */
static void make_worst_scan_order(int n)
{
    for (int r = 0; r < RESOURCES; ++r) {
        available[r] = 0;
    }
    available[0] = 1;

    for (int p = 0; p < n; ++p) {
        for (int r = 0; r < RESOURCES; ++r) {
            allocation[p][r] = 0;
            maximum[p][r] = 0;
        }
        allocation[p][0] = 1;
        maximum[p][0] = (n - p) + 1;
    }
}

static int safety_check(int n)
{
    memcpy(work, available, sizeof(available));
    memset(finish, 0, (size_t)n * sizeof(finish[0]));

    int count = 0;
    while (count < n) {
        int found = 0;

        for (int p = 0; p < n; ++p) {
            if (finish[p]) continue;

            int possible = 1;
            for (int r = 0; r < RESOURCES; ++r) {
                int need = maximum[p][r] - allocation[p][r];
                if (need > work[r]) {
                    possible = 0;
                    break;
                }
            }

            if (possible) {
                for (int r = 0; r < RESOURCES; ++r) {
                    work[r] += allocation[p][r];
                }
                finish[p] = 1;
                sequence[count++] = p;
                found = 1;
            }
        }

        if (!found) return 0;
    }

    return 1;
}

static double median(double v[TRIALS])
{
    double a[TRIALS];
    memcpy(a, v, sizeof(a));
    for (int i = 0; i < TRIALS - 1; ++i) {
        for (int j = i + 1; j < TRIALS; ++j) {
            if (a[j] < a[i]) {
                double t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }
    return a[TRIALS / 2];
}

static int repetitions_for(int n)
{
    if (n == 10) return 2000;
    if (n == 100) return 200;
    return 20;
}

int main(void)
{
    const int sizes[] = {10, 100, 1000};

    printf("Banker's safety-algorithm scaling benchmark\n");
    printf("Resource types=%d; trials=%d; input order designed to "
           "require repeated safety scans.\n", RESOURCES, TRIALS);
    printf("Prediction: worst-case safety scan O(m*n^2), with m=%d.\n\n",
           RESOURCES);
    printf("%-12s %-14s %-18s %-18s\n",
           "Processes", "Repetitions", "Median ms/test", "Result");

    for (size_t s = 0; s < sizeof(sizes)/sizeof(sizes[0]); ++s) {
        int n = sizes[s];
        int reps = repetitions_for(n);
        double samples[TRIALS];

        make_worst_scan_order(n);

        for (int t = 0; t < TRIALS; ++t) {
            double start = monotonic_seconds();
            int all_safe = 1;

            for (int rep = 0; rep < reps; ++rep) {
                if (!safety_check(n)) {
                    all_safe = 0;
                    break;
                }
            }

            samples[t] = (monotonic_seconds() - start) * 1000.0 /
                         (double)reps;

            if (!all_safe) {
                fprintf(stderr, "Generated test state unexpectedly unsafe "
                                "for n=%d.\n", n);
                return EXIT_FAILURE;
            }
        }

        printf("%-12d %-14d %-18.6f %s\n",
               n, reps, median(samples), "SAFE");
    }

    printf("\nTimes are machine-specific medians; do not reuse them as "
           "another computer's measurements.\n");
    return EXIT_SUCCESS;
}
