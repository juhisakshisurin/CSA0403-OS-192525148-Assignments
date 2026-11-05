#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define REQUESTS 10000
#define HDD_LANES 24
#define SSD_LANES 4
#define CACHE_HIT_PERCENT 85

typedef struct {
    double arrival_ms;
    int cache_hit;
    double hdd_service_ms;
} PlaybackRequest;

typedef struct {
    double average_ms;
    double p50_ms;
    double p95_ms;
    double p99_ms;
    double maximum_ms;
    double simulated_seconds;
    double request_rate;
    int completed;
    int cache_hits;
    int cache_misses;
} Metrics;

static uint32_t rng_state = 0x6A09E667U;

static uint32_t next_u32(void)
{
    rng_state = rng_state * 1664525U + 1013904223U;
    return rng_state;
}

static double uniform01(void)
{
    return (double)(next_u32() >> 8) / 16777216.0;
}

static int compare_double(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

static void generate_workload(PlaybackRequest req[REQUESTS])
{
    double time_ms = 0.0;
    rng_state = 0x6A09E667U;

    for (int i = 0; i < REQUESTS; i++) {
        /* 1,000 start/segment requests per second on average:
           inter-arrival is uniformly distributed between 0.5 and 1.5 ms. */
        time_ms += 0.5 + uniform01();

        req[i].arrival_ms = time_ms;
        req[i].cache_hit = ((next_u32() % 100U) < CACHE_HIT_PERCENT);

        /* HDD random-read service time model: 8.0 to 10.0 ms. */
        req[i].hdd_service_ms = 8.0 + 2.0 * uniform01();
    }
}

static double percentile(const double sorted[], int n, double p)
{
    int index = (int)(p * (double)n);
    if (index < 0) index = 0;
    if (index >= n) index = n - 1;
    return sorted[index];
}

static Metrics simulate(const PlaybackRequest req[REQUESTS],
                        int use_ssd_cache)
{
    double hdd_available[HDD_LANES] = {0.0};
    double ssd_available[SSD_LANES] = {0.0};
    double response[REQUESTS];
    double sum = 0.0;
    double last_finish = 0.0;

    Metrics m = {0};

    for (int i = 0; i < REQUESTS; i++) {
        int use_ssd = use_ssd_cache && req[i].cache_hit;
        double start, finish;

        if (use_ssd) {
            int lane = 0;
            for (int j = 1; j < SSD_LANES; j++) {
                if (ssd_available[j] < ssd_available[lane])
                    lane = j;
            }

            start = (ssd_available[lane] > req[i].arrival_ms)
                    ? ssd_available[lane] : req[i].arrival_ms;

            /* NVMe cache read: illustrative 0.25 to 0.40 ms. */
            finish = start + 0.25 + 0.15 *
                     ((double)((next_u32() >> 8) % 10000U) / 10000.0);

            ssd_available[lane] = finish;
            m.cache_hits++;
        } else {
            int lane = 0;
            for (int j = 1; j < HDD_LANES; j++) {
                if (hdd_available[j] < hdd_available[lane])
                    lane = j;
            }

            start = (hdd_available[lane] > req[i].arrival_ms)
                    ? hdd_available[lane] : req[i].arrival_ms;

            finish = start + req[i].hdd_service_ms;
            hdd_available[lane] = finish;

            if (use_ssd_cache)
                m.cache_misses++;
        }

        response[i] = finish - req[i].arrival_ms;
        sum += response[i];
        if (finish > last_finish)
            last_finish = finish;
        m.completed++;
    }

    qsort(response, REQUESTS, sizeof response[0], compare_double);

    m.average_ms = sum / (double)REQUESTS;
    m.p50_ms = percentile(response, REQUESTS, 0.50);
    m.p95_ms = percentile(response, REQUESTS, 0.95);
    m.p99_ms = percentile(response, REQUESTS, 0.99);
    m.maximum_ms = response[REQUESTS - 1];
    m.simulated_seconds = last_finish / 1000.0;
    m.request_rate = (double)REQUESTS /
                     (req[REQUESTS - 1].arrival_ms / 1000.0);

    return m;
}

static void print_metrics(const char *name, const Metrics *m)
{
    printf("\n%s\n", name);
    printf("Requests completed             : %d\n", m->completed);
    printf("Input request arrival rate      : %.2f requests/second\n",
           m->request_rate);
    printf("Average response                : %.3f ms\n", m->average_ms);
    printf("Median response (p50)           : %.3f ms\n", m->p50_ms);
    printf("95th percentile response (p95)  : %.3f ms\n", m->p95_ms);
    printf("99th percentile response (p99)  : %.3f ms\n", m->p99_ms);
    printf("Maximum response                : %.3f ms\n", m->maximum_ms);
    printf("Final completion time           : %.3f seconds\n",
           m->simulated_seconds);

    if (m->cache_hits + m->cache_misses > 0) {
        printf("Hot-cache hits / HDD misses     : %d / %d\n",
               m->cache_hits, m->cache_misses);
    }
}

int main(void)
{
    static PlaybackRequest workload[REQUESTS];

    puts("VIDEO STREAMING START-UP: TWO-DESIGN SIMULATION");
    puts("Model inputs:");
    puts("- 10,000 playback-start/segment requests.");
    puts("- 1,000 requests/second average arrival rate (0.5..1.5 ms intervals).");
    puts("- 24 HDD service lanes, each random read 8..10 ms.");
    puts("- Hybrid design: 4 SSD lanes, each read 0.25..0.40 ms.");
    puts("- Hot-cache hit probability: 85%; misses are served by HDD.");
    puts("- One request represents the first segment read for playback.");
    puts("- These are simulated parameters, not physical benchmark values.");

    generate_workload(workload);

    Metrics hdd = simulate(workload, 0);
    print_metrics("DESIGN A - HDD-ONLY ORIGIN (24 HDD LANES)", &hdd);

    /* Reseed before hybrid run so SSD service jitter is deterministic. */
    rng_state = 0xBB67AE85U;
    Metrics hybrid = simulate(workload, 1);
    print_metrics("DESIGN B - SSD HOT CACHE + HDD ORIGIN", &hybrid);

    printf("\nCOMPARISON\n");
    printf("Average response improvement      : %.2fx\n",
           hdd.average_ms / hybrid.average_ms);
    printf("p95 response improvement          : %.2fx\n",
           hdd.p95_ms / hybrid.p95_ms);
    printf("Average response reduction        : %.1f%%\n",
           (1.0 - hybrid.average_ms / hdd.average_ms) * 100.0);
    puts("\nNote: queueing is represented by earliest-available device lanes.");
    puts("This simplified model does not emulate a complete operating system,");
    puts("network stack, encoder, CDN, real disk firmware or multi-client TCP.");

    return EXIT_SUCCESS;
}
