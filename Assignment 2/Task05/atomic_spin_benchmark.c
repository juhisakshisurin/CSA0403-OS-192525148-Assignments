#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ITERS_PER_THREAD 200000
#define TRIALS 3
enum Mode { ATOMIC_MODE, SPIN_MODE, MUTEX_MODE };

static atomic_long atomic_counter;
static long spin_counter;
static long mutex_counter;
static atomic_flag spin = ATOMIC_FLAG_INIT;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct {
    enum Mode mode;
    double cpu_ms;
    unsigned long long failed_spins;
} Arg;

static double clock_seconds(clockid_t id)
{
    struct timespec ts;
    clock_gettime(id, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void *worker(void *opaque)
{
    Arg *a = (Arg *)opaque;
    double cpu_start = clock_seconds(CLOCK_THREAD_CPUTIME_ID);
    unsigned long long local_failed_spins = 0;

    for (int i = 0; i < ITERS_PER_THREAD; ++i) {
        if (a->mode == ATOMIC_MODE) {
            atomic_fetch_add_explicit(&atomic_counter, 1, memory_order_relaxed);
        } else if (a->mode == SPIN_MODE) {
            while (atomic_flag_test_and_set_explicit(&spin, memory_order_acquire)) {
                ++local_failed_spins;
                /* Yield to reduce monopolisation while still measuring
                   the CPU cost of repeated lock polling. */
                sched_yield();
            }
            ++spin_counter;
            atomic_flag_clear_explicit(&spin, memory_order_release);
        } else {
            pthread_mutex_lock(&mutex);
            ++mutex_counter;
            pthread_mutex_unlock(&mutex);
        }
    }

    a->cpu_ms = (clock_seconds(CLOCK_THREAD_CPUTIME_ID) - cpu_start) * 1000.0;
    a->failed_spins = local_failed_spins;
    return NULL;
}

static double median3(double v[TRIALS])
{
    double a[TRIALS];
    memcpy(a, v, sizeof(a));
    for (int i=0;i<TRIALS-1;i++)
        for (int j=i+1;j<TRIALS;j++)
            if(a[j]<a[i]){double t=a[i];a[i]=a[j];a[j]=t;}
    return a[1];
}

static const char *name(enum Mode m)
{
    return m==ATOMIC_MODE ? "C11 atomic_fetch_add" :
           m==SPIN_MODE ? "atomic_flag spin lock" : "pthread mutex";
}

int main(void)
{
    const int thread_counts[] = {1,4,16};
    const enum Mode modes[] = {ATOMIC_MODE,SPIN_MODE,MUTEX_MODE};

    printf("Shared counter benchmark: atomic vs spin lock vs mutex\n");
    printf("Iterations per thread=%d; trials=%d\n\n",ITERS_PER_THREAD,TRIALS);
    printf("%-24s %7s %14s %14s %14s %18s\n",
           "Method","Threads","Median ms","ns/update","Worker CPU ms","Failed spins");

    for(int ti=0;ti<3;ti++){
        int nthreads=thread_counts[ti];
        for(int mi=0;mi<3;mi++){
            double wall[TRIALS],cpu[TRIALS];
            double fail_as_double[TRIALS] = {0};

            for(int trial=0;trial<TRIALS;trial++){
                atomic_store(&atomic_counter,0);
                spin_counter=0;
                mutex_counter=0;
                atomic_flag_clear(&spin);

                pthread_t th[16];
                Arg args[16];
                double start=clock_seconds(CLOCK_MONOTONIC);

                for(int i=0;i<nthreads;i++){
                    args[i].mode=modes[mi];
                    args[i].cpu_ms=0.0;
                    args[i].failed_spins=0;
                    int rc=pthread_create(&th[i],NULL,worker,&args[i]);
                    if(rc!=0){fprintf(stderr,"pthread_create failed: %d\n",rc);return EXIT_FAILURE;}
                }

                double total_cpu=0.0;
                unsigned long long total_failed_spins=0;
                for(int i=0;i<nthreads;i++){
                    pthread_join(th[i],NULL);
                    total_cpu+=args[i].cpu_ms;
                    total_failed_spins+=args[i].failed_spins;
                }

                wall[trial]=(clock_seconds(CLOCK_MONOTONIC)-start)*1000.0;
                cpu[trial]=total_cpu;
                fail_as_double[trial]=(double)total_failed_spins;
            }

            double med=median3(wall);
            double cpu_med=median3(cpu);
            double fail_median=median3(fail_as_double);
            unsigned long long fail_median_int=(unsigned long long)fail_median;
            double updates=(double)nthreads*ITERS_PER_THREAD;
            double ns_per=(med*1e6)/updates;
            long expected=(long)nthreads*ITERS_PER_THREAD;
            long actual=modes[mi]==ATOMIC_MODE ? atomic_load(&atomic_counter) :
                        modes[mi]==SPIN_MODE ? spin_counter : mutex_counter;
            if(actual != expected){
                fprintf(stderr,"Counter validation failed: actual=%ld expected=%ld\\n",
                        actual,expected);
                return EXIT_FAILURE;
            }
            printf("%-24s %7d %14.3f %14.2f %14.3f %18llu\n",
                   name(modes[mi]),nthreads,med,ns_per,cpu_med,fail_median_int);
        }
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
