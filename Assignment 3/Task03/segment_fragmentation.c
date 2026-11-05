#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MEMORY_KB 1024
#define SEGMENT_COUNT 20
#define MIN_SIZE_KB 12
#define MAX_SIZE_KB 72

typedef struct {
    int start;
    int size;
    int loaded;
} Segment;

static unsigned int rng = 0x20250317U;
static unsigned int next_random(void)
{
    rng = rng * 1664525U + 1013904223U;
    return rng;
}

static int first_fit(unsigned char memory[MEMORY_KB], int size)
{
    int run = 0;
    for (int i = 0; i < MEMORY_KB; ++i) {
        if (!memory[i]) {
            ++run;
            if (run == size)
                return i - size + 1;
        } else {
            run = 0;
        }
    }
    return -1;
}

static void occupy(unsigned char memory[MEMORY_KB], int start,
                   int size, unsigned char value)
{
    for (int i = start; i < start + size; ++i)
        memory[i] = value;
}

static void report_holes(const unsigned char memory[MEMORY_KB],
                         int *free_total, int *largest, int *holes)
{
    *free_total = *largest = *holes = 0;
    int run = 0;

    for (int i = 0; i < MEMORY_KB; ++i) {
        if (!memory[i]) {
            ++(*free_total);
            ++run;
        } else {
            if (run > 0) {
                ++(*holes);
                if (run > *largest) *largest = run;
            }
            run = 0;
        }
    }
    if (run > 0) {
        ++(*holes);
        if (run > *largest) *largest = run;
    }
}

int main(void)
{
    unsigned char memory[MEMORY_KB] = {0};
    Segment seg[SEGMENT_COUNT];

    puts("Contiguous segmentation fragmentation experiment");
    puts("Physical memory = 1024 KiB; sizes are deterministic random values.");
    puts("Load 20 segments using first-fit:");

    int loaded = 0;
    for (int i = 0; i < SEGMENT_COUNT; ++i) {
        int size = MIN_SIZE_KB +
                   (int)(next_random() % (MAX_SIZE_KB - MIN_SIZE_KB + 1));
        int start = first_fit(memory, size);

        seg[i].start = start;
        seg[i].size = size;
        seg[i].loaded = (start >= 0);

        if (start >= 0) {
            occupy(memory, start, size, 1);
            ++loaded;
            printf("S%02d size=%2d KiB allocated [%3d,%3d)\n",
                   i, size, start, start + size);
        } else {
            printf("S%02d size=%2d KiB could not be loaded\n", i, size);
        }
    }

    puts("\nRemove every second allocated segment (odd IDs):");
    for (int i = 1; i < SEGMENT_COUNT; i += 2) {
        if (seg[i].loaded) {
            occupy(memory, seg[i].start, seg[i].size, 0);
            seg[i].loaded = 0;
            printf("Removed S%02d (%d KiB)\n", i, seg[i].size);
        }
    }

    int free_total, largest, holes;
    report_holes(memory, &free_total, &largest, &holes);
    printf("\nAfter removal: free=%d KiB, largest hole=%d KiB, "
           "holes=%d, external-fragmentation indicator=%d KiB\n",
           free_total, largest, holes, free_total - largest);

    int replacement_size = largest + 1;
    if (replacement_size <= free_total && replacement_size <= MEMORY_KB) {
        int start = first_fit(memory, replacement_size);
        printf("Test request=%d KiB: total free is sufficient, "
               "but largest hole is %d KiB -> %s\n",
               replacement_size, largest,
               start < 0 ? "FAILS due to external fragmentation"
                         : "fits");
    } else {
        puts("No request larger than the largest hole but within total free "
             "space exists for this generated layout.");
    }

    return 0;
}
