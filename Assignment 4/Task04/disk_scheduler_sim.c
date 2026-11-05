#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define REQUESTS 10000
#define MAX_CYLINDER 199
#define SEEK_BASE_MS 0.20
#define SEEK_PER_CYL_MS 0.020

typedef enum {
    FCFS = 0, SSTF, SCAN, CSCAN, LOOK, CLOOK, ALG_COUNT
} Algorithm;

typedef struct {
    int cylinder;
    double arrival_ms;
} Request;

typedef struct {
    double total_movement;
    double total_wait_ms;
    double max_wait_ms;
    double finish_ms;
    int completed;
} Result;

static const char *algorithm_names[ALG_COUNT] = {
    "FCFS", "SSTF", "SCAN", "C-SCAN", "LOOK", "C-LOOK"
};

static uint32_t rng_state = 0x4A4F4849U;

static uint32_t next_u32(void)
{
    rng_state = rng_state * 1664525U + 1013904223U;
    return rng_state;
}

static double uniform01(void)
{
    return (double)(next_u32() >> 8) / 16777216.0;
}

static void generate_requests(Request requests[REQUESTS],
                             double mean_interarrival_ms,
                             uint32_t seed)
{
    rng_state = seed;
    double arrival = 0.0;

    for (int i = 0; i < REQUESTS; i++) {
        /* Uniform 0.5..1.5 times the mean interval; deterministic seed. */
        double interval = mean_interarrival_ms * (0.5 + uniform01());
        arrival += interval;

        requests[i].arrival_ms = arrival;
        requests[i].cylinder = (int)(next_u32() % (MAX_CYLINDER + 1));
    }
}

static int abs_int(int x)
{
    return x < 0 ? -x : x;
}

static int choose_pending(const Request req[], const int pending[],
                          int count, int head, Algorithm alg, int direction)
{
    int best_pos = -1;
    int best_metric = 0;

    for (int j = 0; j < count; j++) {
        int idx = pending[j];
        int cyl = req[idx].cylinder;
        int metric = 0;
        int eligible = 1;

        switch (alg) {
        case FCFS:
            metric = idx; /* input array is arrival ordered */
            break;

        case SSTF:
            metric = abs_int(cyl - head);
            break;

        case SCAN:
        case CSCAN:
        case LOOK:
        case CLOOK:
            eligible = (direction > 0) ? (cyl >= head) : (cyl <= head);
            metric = abs_int(cyl - head);
            break;

        default:
            return -1;
        }

        if (!eligible)
            continue;

        if (best_pos < 0) {
            best_pos = j;
            best_metric = metric;
        } else if (alg == SSTF) {
            if (metric < best_metric ||
                (metric == best_metric && idx < pending[best_pos])) {
                best_pos = j;
                best_metric = metric;
            }
        } else if (alg == FCFS) {
            if (metric < best_metric) {
                best_pos = j;
                best_metric = metric;
            }
        } else {
            /* Directional sweep: nearest request in the current direction. */
            int best_cyl = req[pending[best_pos]].cylinder;
            if ((direction > 0 && cyl < best_cyl) ||
                (direction < 0 && cyl > best_cyl)) {
                best_pos = j;
                best_metric = metric;
            }
        }
    }

    return best_pos;
}

static void move_empty(Result *res, int *head, int destination)
{
    int distance = abs_int(destination - *head);
    res->total_movement += (double)distance;
    res->finish_ms += (double)distance * SEEK_PER_CYL_MS;
    *head = destination;
}

static Result simulate(const Request requests[REQUESTS], Algorithm alg)
{
    int pending[REQUESTS];
    int pending_count = 0;
    int next_arrival = 0;
    int head = 53;
    int direction = (alg == SCAN || alg == LOOK) ? -1 : 1;
    double now = 0.0;
    Result res = {0};

    while (res.completed < REQUESTS) {
        /* Admit every request that has arrived by the current dispatch time. */
        while (next_arrival < REQUESTS &&
               requests[next_arrival].arrival_ms <= now) {
            pending[pending_count++] = next_arrival++;
        }

        if (pending_count == 0) {
            if (next_arrival >= REQUESTS)
                break;
            now = requests[next_arrival].arrival_ms;
            if (now > res.finish_ms)
                res.finish_ms = now;
            continue;
        }

        int chosen_pos = choose_pending(
            requests, pending, pending_count, head, alg, direction
        );

        if (chosen_pos < 0) {
            /* No request ahead: SCAN goes to an edge; LOOK turns in place.
               C-SCAN wraps to cylinder 0; C-LOOK jumps to the lowest pending. */
            if (alg == SCAN) {
                int edge = (direction > 0) ? MAX_CYLINDER : 0;
                move_empty(&res, &head, edge);
                direction = -direction;
                now = res.finish_ms > now ? res.finish_ms : now;
                continue;
            }

            if (alg == LOOK) {
                direction = -direction;
                continue;
            }

            if (alg == CSCAN) {
                if (direction > 0) {
                    move_empty(&res, &head, MAX_CYLINDER);
                    move_empty(&res, &head, 0);
                } else {
                    move_empty(&res, &head, 0);
                    move_empty(&res, &head, MAX_CYLINDER);
                }
                now = res.finish_ms > now ? res.finish_ms : now;
                continue;
            }

            if (alg == CLOOK) {
                int target_cyl = requests[pending[0]].cylinder;

                for (int j = 1; j < pending_count; j++) {
                    int cyl = requests[pending[j]].cylinder;
                    if ((direction > 0 && cyl < target_cyl) ||
                        (direction < 0 && cyl > target_cyl)) {
                        target_cyl = cyl;
                    }
                }

                move_empty(&res, &head, target_cyl);
                now = res.finish_ms > now ? res.finish_ms : now;
                continue;
            }

            /* FCFS/SSTF always select a pending request. */
            fprintf(stderr, "Unexpected empty selection for %s\n",
                    algorithm_names[alg]);
            exit(EXIT_FAILURE);
        }

        int request_index = pending[chosen_pos];
        int destination = requests[request_index].cylinder;
        double wait = now - requests[request_index].arrival_ms;

        if (wait < 0.0)
            wait = 0.0;

        res.total_wait_ms += wait;
        if (wait > res.max_wait_ms)
            res.max_wait_ms = wait;

        /* Remove selected request from the pending array. */
        pending[chosen_pos] = pending[pending_count - 1];
        pending_count--;

        int distance = abs_int(destination - head);
        res.total_movement += (double)distance;
        now += SEEK_BASE_MS + (double)distance * SEEK_PER_CYL_MS;
        head = destination;
        res.finish_ms = now;
        res.completed++;
    }

    return res;
}

static void print_load(const char *load_name, double interval_ms,
                       uint32_t seed, FILE *csv)
{
    Request requests[REQUESTS];
    generate_requests(requests, interval_ms, seed);

    printf("\n=== %s load: mean inter-arrival %.2f ms, %d requests ===\n",
           load_name, interval_ms, REQUESTS);

    for (int alg = 0; alg < ALG_COUNT; alg++) {
        Result r = simulate(requests, (Algorithm)alg);
        double avg_movement = r.total_movement / (double)r.completed;
        double avg_wait = r.total_wait_ms / (double)r.completed;

        printf("%-7s avg_head_movement=%8.3f cylinders/request"
               " avg_wait=%10.3f ms max_wait=%12.3f ms\n",
               algorithm_names[alg], avg_movement,
               avg_wait, r.max_wait_ms);

        fprintf(csv, "%s,%s,%d,%.3f,%.6f,%.6f,%.6f,%.6f\n",
                load_name, algorithm_names[alg], r.completed,
                interval_ms, avg_movement, avg_wait,
                r.max_wait_ms, r.finish_ms);
    }
}

static void demonstrate_sstf_starvation(void)
{
    const int STREAM_COUNT = 1000;
    int near_cylinders[STREAM_COUNT];
    int pending[STREAM_COUNT];
    int next = 0, pending_count = 0, served = 0;
    int head = 53;
    double now = 0.0;

    for (int i = 0; i < STREAM_COUNT; i++)
        near_cylinders[i] = 59 + (i % 3); /* 59, 60, 61 */

    /* A request at 183 is already waiting at time zero. */
    const int far_request = 183;
    const double far_arrival = 0.0;

    /* The first near request also arrives at time zero; then one every 1 ms. */
    while (served < STREAM_COUNT) {
        while (next < STREAM_COUNT && (double)next <= now) {
            pending[pending_count++] = next;
            next++;
        }

        if (pending_count == 0) {
            now = (double)next;
            continue;
        }

        /* SSTF always selects the nearest currently pending near request.
           Each near request is 0-8 cylinders away, while 183 is over 120 away. */
        int best_pos = 0;
        int best_distance = abs_int(near_cylinders[pending[0]] - head);

        for (int j = 1; j < pending_count; j++) {
            int index = pending[j];
            int distance = abs_int(near_cylinders[index] - head);
            if (distance < best_distance) {
                best_distance = distance;
                best_pos = j;
            }
        }

        int chosen = pending[best_pos];
        pending[best_pos] = pending[pending_count - 1];
        pending_count--;

        int destination = near_cylinders[chosen];
        int movement = abs_int(destination - head);

        /* Illustrative busy-device service time: nearby request service
           exceeds the 1 ms arrival interval, keeping near requests queued. */
        now += 1.20 + 0.030 * (double)movement;
        head = destination;
        served++;
    }

    double far_wait = now - far_arrival;
    printf("\nSSTF near-stream fairness demonstration:\n");
    printf("- %d requests at cylinders 59/60/61 arrive every 1 ms.\n",
           STREAM_COUNT);
    printf("- A request at cylinder %d is waiting from time 0.\n", far_request);
    printf("- SSTF serves all %d nearer requests before cylinder %d.\n",
           STREAM_COUNT, far_request);
    printf("- Modelled wait for cylinder %d: %.3f ms (finite stream).\n",
           far_request, far_wait);
    puts("- If the closer stream continues indefinitely, the far request can starve.");
}

int main(void)
{
    FILE *csv = fopen("scheduler_simulation.csv", "w");
    if (csv == NULL) {
        perror("scheduler_simulation.csv");
        return EXIT_FAILURE;
    }

    fprintf(csv,
        "Load,Algorithm,Requests,MeanInterarrival_ms,"
        "AverageHeadMovement_cyl_per_request,AverageWaiting_ms,"
        "MaximumWaiting_ms,TotalSimulatedTime_ms\n"
    );

    puts("Disk scheduling simulation assumptions:");
    puts("- Cylinders: 0..199; initial head: 53.");
    puts("- 10,000 random requests per load; uniform cylinder selection.");
    puts("- Arrival intervals are uniform in [0.5, 1.5] x the stated mean.");
    puts("- Light/medium/heavy mean inter-arrival: 8.0 / 2.0 / 0.6 ms.");
    puts("- Modelled service time: 0.20 + 0.020 x head movement (ms).");
    puts("- Waiting time = dispatch start time - request arrival time.");
    puts("- Average movement includes SCAN/C-SCAN/C-LOOK travel and wrap movement.");

    print_load("Light", 8.0, 0x11111111U, csv);
    print_load("Medium", 2.0, 0x22222222U, csv);
    print_load("Heavy", 0.6, 0x33333333U, csv);

    fclose(csv);
    demonstrate_sstf_starvation();
    puts("\nSaved scheduler_simulation.csv.");
    return EXIT_SUCCESS;
}
