#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 5
#define ITEMS 20
#define MAX_PRODUCERS 4
#define MAX_CONSUMERS 4
#define TOTAL_MAX (ITEMS * MAX_PRODUCERS)

static int buffer[SIZE];
static int in = 0, out = 0;
static sem_t empty_slots, full_slots;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static atomic_long consumed_sum;
static atomic_int seen[TOTAL_MAX + 1];

typedef struct {
    int id;
    int items_to_consume;
} ThreadArg;

static int sem_wait_retry(sem_t *sem)
{
    while (sem_wait(sem) == -1) {
        if (errno == EINTR) continue;
        return -1;
    }
    return 0;
}

static void *producer(void *arg)
{
    ThreadArg *a = (ThreadArg *)arg;

    for (int item = 1; item <= ITEMS; ++item) {
        int value = a->id * ITEMS + item;

        /* TODO 1: wait for an empty slot, protect the circular-buffer
           update, then signal that one full slot is available. */
        if (sem_wait_retry(&empty_slots) != 0) {
            perror("producer sem_wait");
            return NULL;
        }

#ifndef UNSAFE_NO_MUTEX
        pthread_mutex_lock(&lock);
#endif
        buffer[in] = value;
        in = (in + 1) % SIZE;
#ifndef UNSAFE_NO_MUTEX
        pthread_mutex_unlock(&lock);
#endif

        if (sem_post(&full_slots) == -1) {
            perror("producer sem_post");
            return NULL;
        }
    }

    return NULL;
}

static void *consumer(void *arg)
{
    ThreadArg *a = (ThreadArg *)arg;

    for (int k = 0; k < a->items_to_consume; ++k) {
        int item;

        /* TODO 2: wait for a full slot, protect removal from the
           circular buffer, signal an empty slot, and account for item. */
        if (sem_wait_retry(&full_slots) != 0) {
            perror("consumer sem_wait");
            return NULL;
        }

#ifndef UNSAFE_NO_MUTEX
        pthread_mutex_lock(&lock);
#endif
        item = buffer[out];
        out = (out + 1) % SIZE;
#ifndef UNSAFE_NO_MUTEX
        pthread_mutex_unlock(&lock);
#endif

        if (sem_post(&empty_slots) == -1) {
            perror("consumer sem_post");
            return NULL;
        }

        /* Atomic accounting protects the sum even in the unsafe-index
           experiment; it does NOT protect buffer[in]/buffer[out]. */
        atomic_fetch_add_explicit(&consumed_sum, item, memory_order_relaxed);
        if (item >= 1 && item <= TOTAL_MAX) {
            atomic_fetch_add_explicit(&seen[item], 1, memory_order_relaxed);
        }
    }

    return NULL;
}

static int run_configuration(int producer_count, int consumer_count,
                             int verbose)
{
    if (producer_count < 1 || producer_count > MAX_PRODUCERS ||
        consumer_count < 1 || consumer_count > MAX_CONSUMERS ||
        (producer_count * ITEMS) % consumer_count != 0) {
        fprintf(stderr, "Use producers/consumers between 1 and 4 with "
                        "a divisible total item count.\n");
        return -1;
    }

    memset(buffer, 0, sizeof(buffer));
    in = out = 0;
    atomic_store(&consumed_sum, 0);
    for (int i = 0; i <= TOTAL_MAX; ++i) atomic_store(&seen[i], 0);

    if (sem_init(&empty_slots, 0, SIZE) != 0) {
        perror("sem_init empty");
        return -1;
    }
    if (sem_init(&full_slots, 0, 0) != 0) {
        perror("sem_init full");
        sem_destroy(&empty_slots);
        return -1;
    }

    pthread_t producers[MAX_PRODUCERS], consumers[MAX_CONSUMERS];
    ThreadArg pargs[MAX_PRODUCERS], cargs[MAX_CONSUMERS];
    int total_items = producer_count * ITEMS;
    int each_consumer = total_items / consumer_count;
    int made_p = 0, made_c = 0;

    for (int i = 0; i < producer_count; ++i) {
        pargs[i].id = i;
        pargs[i].items_to_consume = 0;
        int rc = pthread_create(&producers[i], NULL, producer, &pargs[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create producer: %s\n", strerror(rc));
            goto creation_error;
        }
        ++made_p;
    }

    for (int i = 0; i < consumer_count; ++i) {
        cargs[i].id = i;
        cargs[i].items_to_consume = each_consumer;
        int rc = pthread_create(&consumers[i], NULL, consumer, &cargs[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create consumer: %s\n", strerror(rc));
            goto creation_error;
        }
        ++made_c;
    }

    for (int i = 0; i < producer_count; ++i) {
        if (pthread_join(producers[i], NULL) != 0) {
            fprintf(stderr, "Could not join producer.\n");
            goto cleanup;
        }
    }
    for (int i = 0; i < consumer_count; ++i) {
        if (pthread_join(consumers[i], NULL) != 0) {
            fprintf(stderr, "Could not join consumer.\n");
            goto cleanup;
        }
    }

    long expected_sum = (long)total_items * (total_items + 1) / 2;
    /* IDs are contiguous 1..total_items across the producer set. */
    int exact_once = 1;
    for (int i = 1; i <= total_items; ++i) {
        if (atomic_load(&seen[i]) != 1) {
            exact_once = 0;
            break;
        }
    }

    if (verbose) {
        printf("producers=%d consumers=%d items=%d sum=%ld (expected %ld) "
               "each item exactly once: %s\n",
               producer_count, consumer_count, total_items,
               atomic_load(&consumed_sum), expected_sum,
               exact_once && atomic_load(&consumed_sum) == expected_sum
                   ? "PASS" : "FAIL");
    }

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    return (exact_once && atomic_load(&consumed_sum) == expected_sum) ? 0 : 1;

creation_error:
    /* This path is only for resource-creation failure. A partially started
       producer/consumer set may need cancellation; stop and join what exists. */
    for (int i = 0; i < made_p; ++i) pthread_cancel(producers[i]);
    for (int i = 0; i < made_c; ++i) pthread_cancel(consumers[i]);
    for (int i = 0; i < made_p; ++i) pthread_join(producers[i], NULL);
    for (int i = 0; i < made_c; ++i) pthread_join(consumers[i], NULL);
cleanup:
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    return -1;
}

int main(int argc, char **argv)
{
    int p = 1, c = 1;

    if (argc == 3) {
        p = atoi(argv[1]);
        c = atoi(argv[2]);
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [producer_count consumer_count]\n", argv[0]);
        return EXIT_FAILURE;
    }

#ifdef UNSAFE_NO_MUTEX
    printf("WARNING: built without buffer mutex; multi-producer/multi-consumer "
           "index accesses have data races.\n");
#endif

    if (run_configuration(p, c, 1) != 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
