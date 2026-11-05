#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CAMERA_COUNT 64
#define FRAMES_PER_CAMERA 4
#define WORK_PER_FRAME 200000
#define TRIALS 5

typedef struct {
    int camera_id;
    uint64_t *results;
} CameraArg;

typedef struct {
    atomic_int next_camera;
    uint64_t *results;
} PoolContext;

typedef struct {
    const char *name;
    int threads;
    double median_ms;
    uint64_t checksum;
} Measurement;

static uint64_t process_frame(int camera_id, int frame_id)
{
    /* Deterministic CPU workload used as a stand-in for frame analysis.
       It is NOT an actual image decoder or number-plate recognition model. */
    uint64_t x = UINT64_C(0x9e3779b97f4a7c15)
               ^ ((uint64_t)(camera_id + 1) * UINT64_C(0xbf58476d1ce4e5b9))
               ^ ((uint64_t)(frame_id + 1) * UINT64_C(0x94d049bb133111eb));

    for (int i = 0; i < WORK_PER_FRAME; ++i) {
        x ^= x >> 12;
        x ^= x << 25;
        x ^= x >> 27;
        x *= UINT64_C(0x2545F4914F6CDD1D);
        x += (uint64_t)i + (uint64_t)camera_id;
    }
    return x;
}

static uint64_t process_camera(int camera_id)
{
    uint64_t total = 0;
    for (int frame = 0; frame < FRAMES_PER_CAMERA; ++frame) {
        total ^= process_frame(camera_id, frame);
        total = (total << 7) | (total >> (64 - 7));
    }
    return total;
}

static void *one_camera_worker(void *opaque)
{
    CameraArg *arg = (CameraArg *)opaque;
    arg->results[arg->camera_id] = process_camera(arg->camera_id);
    return NULL;
}

static void *pool_worker(void *opaque)
{
    PoolContext *ctx = (PoolContext *)opaque;

    for (;;) {
        int camera = atomic_fetch_add(&ctx->next_camera, 1);
        if (camera >= CAMERA_COUNT) {
            break;
        }
        /* Each worker writes to a distinct results[camera] element. */
        ctx->results[camera] = process_camera(camera);
    }
    return NULL;
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

static uint64_t combined_checksum(const uint64_t results[CAMERA_COUNT])
{
    uint64_t sum = 0;
    for (int i = 0; i < CAMERA_COUNT; ++i) {
        sum ^= results[i] + (uint64_t)i;
        sum = (sum << 9) | (sum >> (64 - 9));
    }
    return sum;
}

static int run_sequential(uint64_t results[CAMERA_COUNT], double *elapsed_ms)
{
    double start = now_seconds();
    for (int camera = 0; camera < CAMERA_COUNT; ++camera) {
        results[camera] = process_camera(camera);
    }
    *elapsed_ms = (now_seconds() - start) * 1000.0;
    return 0;
}

static int run_64_threads(uint64_t results[CAMERA_COUNT], double *elapsed_ms)
{
    pthread_t threads[CAMERA_COUNT];
    CameraArg args[CAMERA_COUNT];
    int created = 0;

    double start = now_seconds();
    for (int i = 0; i < CAMERA_COUNT; ++i) {
        args[i].camera_id = i;
        args[i].results = results;
        int rc = pthread_create(&threads[i], NULL, one_camera_worker, &args[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create for camera %d: %s\n", i, strerror(rc));
            for (int j = 0; j < created; ++j) {
                (void)pthread_join(threads[j], NULL);
            }
            return -1;
        }
        ++created;
    }

    for (int i = 0; i < created; ++i) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(rc));
            return -1;
        }
    }
    *elapsed_ms = (now_seconds() - start) * 1000.0;
    return 0;
}

static int run_thread_pool(int thread_count,
                           uint64_t results[CAMERA_COUNT],
                           double *elapsed_ms)
{
    pthread_t threads[16];
    PoolContext ctx;
    atomic_init(&ctx.next_camera, 0);
    ctx.results = results;
    int created = 0;

    double start = now_seconds();
    for (int i = 0; i < thread_count; ++i) {
        int rc = pthread_create(&threads[i], NULL, pool_worker, &ctx);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            for (int j = 0; j < created; ++j) {
                (void)pthread_join(threads[j], NULL);
            }
            return -1;
        }
        ++created;
    }

    for (int i = 0; i < created; ++i) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(rc));
            return -1;
        }
    }
    *elapsed_ms = (now_seconds() - start) * 1000.0;
    return 0;
}

static double median(double values[TRIALS])
{
    double copy[TRIALS];
    memcpy(copy, values, sizeof(copy));
    for (int i = 0; i < TRIALS - 1; ++i) {
        for (int j = i + 1; j < TRIALS; ++j) {
            if (copy[j] < copy[i]) {
                double tmp = copy[i];
                copy[i] = copy[j];
                copy[j] = tmp;
            }
        }
    }
    return copy[TRIALS / 2];
}

/* Demonstrates the correct way to update a shared counter. */
#define COUNTER_THREADS 4
#define INCREMENTS_PER_THREAD 100000

static long protected_counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *counter_worker(void *unused)
{
    (void)unused;
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        pthread_mutex_lock(&counter_mutex);
        ++protected_counter;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

static int run_counter_check(void)
{
    pthread_t threads[COUNTER_THREADS];
    for (int i = 0; i < COUNTER_THREADS; ++i) {
        int rc = pthread_create(&threads[i], NULL, counter_worker, NULL);
        if (rc != 0) {
            fprintf(stderr, "counter pthread_create: %s\n", strerror(rc));
            return -1;
        }
    }
    for (int i = 0; i < COUNTER_THREADS; ++i) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "counter pthread_join: %s\n", strerror(rc));
            return -1;
        }
    }

    long expected = (long)COUNTER_THREADS * INCREMENTS_PER_THREAD;
    printf("\nMutex-protected counter: actual=%ld, expected=%ld, %s\n",
           protected_counter, expected,
           protected_counter == expected ? "PASS" : "FAIL");
    pthread_mutex_destroy(&counter_mutex);
    return protected_counter == expected ? 0 : -1;
}

int main(void)
{
    uint64_t baseline[CAMERA_COUNT] = {0};
    uint64_t current[CAMERA_COUNT] = {0};
    double samples[TRIALS];
    Measurement m[5] = {
        {"Sequential", 1, 0.0, 0},
        {"One thread/camera", 64, 0.0, 0},
        {"Pool of 4", 4, 0.0, 0},
        {"Pool of 8", 8, 0.0, 0},
        {"Pool of 16", 16, 0.0, 0}
    };

    printf("Traffic-camera threading benchmark\n");
    printf("Cameras=%d, frames/camera=%d, work/frame=%d, trials=%d\n",
           CAMERA_COUNT, FRAMES_PER_CAMERA, WORK_PER_FRAME, TRIALS);
    printf("Workload is synthetic CPU work, not real image recognition.\n\n");

    for (int trial = 0; trial < TRIALS; ++trial) {
        if (run_sequential(current, &samples[trial]) != 0) return EXIT_FAILURE;
        if (trial == 0) memcpy(baseline, current, sizeof(baseline));
    }
    m[0].median_ms = median(samples);
    m[0].checksum = combined_checksum(baseline);

    for (int strategy = 1; strategy < 5; ++strategy) {
        for (int trial = 0; trial < TRIALS; ++trial) {
            int rc;
            if (strategy == 1) {
                rc = run_64_threads(current, &samples[trial]);
            } else {
                rc = run_thread_pool(m[strategy].threads, current, &samples[trial]);
            }
            if (rc != 0) return EXIT_FAILURE;

            if (memcmp(current, baseline, sizeof(baseline)) != 0) {
                fprintf(stderr, "Result mismatch in %s configuration.\n",
                        m[strategy].name);
                return EXIT_FAILURE;
            }
        }
        m[strategy].median_ms = median(samples);
        m[strategy].checksum = combined_checksum(current);
    }

    printf("%-22s %14s %12s %18s\n",
           "Configuration", "Median ms", "Speed-up", "Checksum");
    for (int i = 0; i < 5; ++i) {
        double speedup = m[0].median_ms / m[i].median_ms;
        printf("%-22s %14.3f %11.3fx %18" PRIu64 "\n",
               m[i].name, m[i].median_ms, speedup, m[i].checksum);
    }
    printf("\nVerification: all five configurations produced identical per-camera results.\n");
    printf("Speed-up = sequential median time / configuration median time.\n");
    printf("Thread creation and joining are included in threaded timings.\n");

    if (run_counter_check() != 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
