#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 200000

static atomic_int flag[2];
static atomic_int turn;
static long protected_counter = 0;

static void peterson_lock(int self)
{
    int other = 1 - self;
    atomic_store_explicit(&flag[self], 1, memory_order_seq_cst);
    atomic_store_explicit(&turn, other, memory_order_seq_cst);

    while (atomic_load_explicit(&flag[other], memory_order_seq_cst) &&
           atomic_load_explicit(&turn, memory_order_seq_cst) == other) {
        sched_yield();
    }
}

static void peterson_unlock(int self)
{
    atomic_store_explicit(&flag[self], 0, memory_order_seq_cst);
}

static void *worker(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < ITERATIONS; ++i) {
        peterson_lock(id);
        ++protected_counter;
        peterson_unlock(id);
    }
    return NULL;
}

int main(void)
{
    pthread_t t[2];
    int ids[2] = {0, 1};

    atomic_init(&flag[0], 0);
    atomic_init(&flag[1], 0);
    atomic_init(&turn, 0);

    for (int i = 0; i < 2; ++i) {
        int rc = pthread_create(&t[i], NULL, worker, &ids[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < 2; ++i) pthread_join(t[i], NULL);

    long expected = 2L * ITERATIONS;
    printf("Peterson with C11 sequentially-consistent atomics: "
           "actual=%ld expected=%ld %s\n",
           protected_counter, expected,
           protected_counter == expected ? "PASS" : "FAIL");
    return protected_counter == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}
