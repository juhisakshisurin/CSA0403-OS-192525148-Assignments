#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNIT_MIB 16
#define TOTAL_RAM_MIB 2048
#define MAX_UNITS (TOTAL_RAM_MIB / UNIT_MIB)
#define MAX_REF 20000

typedef struct {
    const char *name;
    int memory_mib;
    int frames;
    int pinned;
} TaskPlan;

static int contains(const int *frames, int used, int page)
{
    for (int i = 0; i < used; ++i)
        if (frames[i] == page)
            return i;
    return -1;
}

/* LRU simulator: one abstract page is 16 MiB for this budget model.
   This is not the hardware page size used by Linux. */
static int simulate_lru(const int *refs, int count, int frame_count,
                        int preload, int *runtime_faults)
{
    int frames[MAX_UNITS];
    int last[MAX_UNITS];
    int used = 0;
    int faults = 0;

    if (preload) {
        for (int p = 0; p < preload && p < frame_count; ++p) {
            frames[used] = p;
            last[used] = -1;
            used++;
        }
    }

    for (int i = 0; i < count; ++i) {
        int page = refs[i];
        int slot = contains(frames, used, page);

        if (slot >= 0) {
            last[slot] = i;
            continue;
        }

        faults++;

        int victim;
        if (used < frame_count) {
            victim = used++;
            frames[victim] = page;
        } else {
            victim = 0;
            for (int j = 1; j < frame_count; ++j) {
                if (last[j] < last[victim])
                    victim = j;
            }
            frames[victim] = page;
        }

        last[victim] = i;
    }

    *runtime_faults = faults;
    return faults;
}

static void make_cyclic_refs(int *refs, int count, int working_pages,
                             int phase)
{
    for (int i = 0; i < count; ++i)
        refs[i] = (i + phase) % working_pages;
}

int main(void)
{
    /*
       Explicit design budget for a minimal headless edge Linux image:
       OS/kernel/services 320 MiB
       Real-time locked working set 64 MiB
       ML task 1.5 GiB = 1536 MiB
       Logger active working set 64 MiB
       Safety/headroom 64 MiB
       Total = 2048 MiB.
    */
    const int os_mib = 320;
    const int rt_mib = 64;
    const int ml_mib = 1536;
    const int log_mib = 64;
    const int headroom_mib = 64;

    TaskPlan plan[] = {
        {"Real-time control", rt_mib, rt_mib / UNIT_MIB, 1},
        {"Machine-learning", ml_mib, ml_mib / UNIT_MIB, 0},
        {"Logging", log_mib, log_mib / UNIT_MIB, 0}
    };

    printf("2 GiB edge-device memory budget (abstract %d MiB units)\n",
           UNIT_MIB);
    printf("OS/kernel/services : %4d MiB\n", os_mib);
    printf("Real-time locked   : %4d MiB\n", rt_mib);
    printf("Machine-learning  : %4d MiB\n", ml_mib);
    printf("Logging working set: %4d MiB\n", log_mib);
    printf("Safety headroom    : %4d MiB\n", headroom_mib);

    int total = os_mib + rt_mib + ml_mib + log_mib + headroom_mib;
    printf("Total planned      : %4d MiB / %d MiB\n", total, TOTAL_RAM_MIB);

    if (total > TOTAL_RAM_MIB) {
        puts("ERROR: plan exceeds physical RAM.");
        return EXIT_FAILURE;
    }

    printf("\nPage-replacement simulation (each model page = %d MiB).\n",
           UNIT_MIB);
    puts("The simulator uses per-task LRU frame allocations.");
    puts("The real-time pages are preloaded/pinned before the measured run.");

    int references[MAX_REF];
    int count = 0;

    /* Real-time task: preload the four model pages, then cycle through them. */
    make_cyclic_refs(references, 10000, plan[0].frames, 0);
    int rt_faults = 0;
    simulate_lru(references, 10000, plan[0].frames,
                 plan[0].frames, &rt_faults);
    printf("%-20s working-set=%4d MiB frames=%3d "
           "initial_prefaults=%3d runtime_page_faults=%3d\n",
           plan[0].name, plan[0].memory_mib, plan[0].frames,
           plan[0].frames, rt_faults);

    /* ML: reference all 96 model pages in repeated cycles. */
    count = plan[1].frames * 8;
    make_cyclic_refs(references, count, plan[1].frames, 0);
    int ml_faults = 0;
    simulate_lru(references, count, plan[1].frames, 0, &ml_faults);
    printf("%-20s working-set=%4d MiB frames=%3d "
           "simulated_demand_faults=%4d\n",
           plan[1].name, plan[1].memory_mib, plan[1].frames, ml_faults);

    /* Logger: four-page ring buffer repeatedly reused. */
    count = plan[2].frames * 100;
    make_cyclic_refs(references, count, plan[2].frames, 0);
    int log_faults = 0;
    simulate_lru(references, count, plan[2].frames, 0, &log_faults);
    printf("%-20s working-set=%4d MiB frames=%3d "
           "simulated_demand_faults=%4d\n",
           plan[2].name, plan[2].memory_mib, plan[2].frames, log_faults);

    puts("\nVerification:");
    if (rt_faults == 0)
        puts("PASS: real-time task has zero page faults during the simulated control phase.");
    else
        puts("FAIL: real-time task experienced replacement faults.");

    if (ml_faults == plan[1].frames)
        puts("ML task faults only on first touch; its active model working set fits in its allocated frames.");
    else
        puts("ML task replacement occurred beyond first-touch.");

    if (log_faults == plan[2].frames)
        puts("Logger ring buffer faults only on initial fill; subsequent reuse is resident.");
    else
        puts("Logger experienced replacement faults during the simulated reuse phase.");

    puts("\nFourth-task admission check:");
    const int new_task_mib = 500;
    int new_total = total + new_task_mib;
    printf("Adding fourth task: %d MiB -> planned total %d MiB.\n",
           new_task_mib, new_total);
    if (new_total > TOTAL_RAM_MIB) {
        printf("REJECT on 2 GiB device: exceeds RAM by %d MiB before extra safety margin.\n",
               new_total - TOTAL_RAM_MIB);
        puts("Recommendation: upgrade to at least 3 GiB; 4 GiB is a more practical target.");
    } else {
        puts("Fourth task fits within the current memory plan.");
    }

    return 0;
}
