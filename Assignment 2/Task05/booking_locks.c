#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRAINS 8
#define COACHES 8
#define SEATS 512
#define THREADS 16
#define ATTEMPTS_PER_THREAD 10000
#define TRIALS 3

enum LockMode { GLOBAL_LOCK, TRAIN_LOCK, COACH_LOCK };

static unsigned char booked[TRAINS][COACHES][SEATS];
static pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t train_lock[TRAINS];
static pthread_mutex_t coach_lock[TRAINS][COACHES];

typedef struct {
    int id;
    enum LockMode mode;
    unsigned int successes;
} WorkerArg;

static double seconds_now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static uint32_t next_random(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static pthread_mutex_t *select_lock(enum LockMode mode, int train, int coach)
{
    if (mode == GLOBAL_LOCK) return &global_lock;
    if (mode == TRAIN_LOCK) return &train_lock[train];
    return &coach_lock[train][coach];
}

static void *agent(void *opaque)
{
    WorkerArg *a = (WorkerArg *)opaque;
    uint32_t rng = 0x9e3779b9U ^ (uint32_t)(a->id + 1) * 0x85ebca6bU;

    for (int i = 0; i < ATTEMPTS_PER_THREAD; ++i) {
        int train = (int)(next_random(&rng) % TRAINS);
        int coach = (int)(next_random(&rng) % COACHES);
        int seat = (int)(next_random(&rng) % SEATS);

        pthread_mutex_t *m = select_lock(a->mode, train, coach);
        pthread_mutex_lock(m);

        if (!booked[train][coach][seat]) {
            booked[train][coach][seat] = 1;
            a->successes++;
        }

        pthread_mutex_unlock(m);
    }
    return NULL;
}

static double median3(double x[TRIALS])
{
    double v[TRIALS];
    memcpy(v, x, sizeof(v));
    for (int i = 0; i < TRIALS-1; ++i)
        for (int j = i+1; j < TRIALS; ++j)
            if (v[j] < v[i]) { double t=v[i]; v[i]=v[j]; v[j]=t; }
    return v[1];
}

static const char *mode_name(enum LockMode m)
{
    if (m == GLOBAL_LOCK) return "One global lock";
    if (m == TRAIN_LOCK) return "One lock per train";
    return "One lock per coach";
}

int main(void)
{
    for (int t = 0; t < TRAINS; ++t) {
        pthread_mutex_init(&train_lock[t], NULL);
        for (int c = 0; c < COACHES; ++c)
            pthread_mutex_init(&coach_lock[t][c], NULL);
    }

    const enum LockMode modes[] = {GLOBAL_LOCK, TRAIN_LOCK, COACH_LOCK};

    printf("Railway booking lock-granularity benchmark\n");
    printf("Trains=%d Coaches/train=%d Seats/coach=%d Threads=%d "
           "Attempts/thread=%d Trials=%d\n\n",
           TRAINS, COACHES, SEATS, THREADS, ATTEMPTS_PER_THREAD, TRIALS);
    printf("%-22s %14s %16s %18s\n",
           "Lock strategy", "Median ms", "Requests/sec", "Successful bookings");

    for (int k = 0; k < 3; ++k) {
        double times[TRIALS];
        unsigned long last_success = 0;

        for (int trial = 0; trial < TRIALS; ++trial) {
            memset(booked, 0, sizeof(booked));
            pthread_t threads[THREADS];
            WorkerArg args[THREADS];

            double start = seconds_now();
            for (int i = 0; i < THREADS; ++i) {
                args[i].id = i;
                args[i].mode = modes[k];
                args[i].successes = 0;
                int rc = pthread_create(&threads[i], NULL, agent, &args[i]);
                if (rc != 0) {
                    fprintf(stderr, "pthread_create failed: %d\n", rc);
                    return EXIT_FAILURE;
                }
            }

            unsigned long success = 0;
            for (int i = 0; i < THREADS; ++i) {
                pthread_join(threads[i], NULL);
                success += args[i].successes;
            }
            times[trial] = (seconds_now() - start) * 1000.0;
            last_success = success;
        }

        double med = median3(times);
        double attempts = (double)THREADS * ATTEMPTS_PER_THREAD;
        double requests_per_sec = attempts / (med / 1000.0);

        printf("%-22s %14.3f %16.0f %18lu\n",
               mode_name(modes[k]), med, requests_per_sec, last_success);
    }

    for (int t = 0; t < TRAINS; ++t) {
        pthread_mutex_destroy(&train_lock[t]);
        for (int c = 0; c < COACHES; ++c)
            pthread_mutex_destroy(&coach_lock[t][c]);
    }
    pthread_mutex_destroy(&global_lock);
    return 0;
}
