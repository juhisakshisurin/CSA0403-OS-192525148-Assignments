#include <stdio.h>
#include <stdlib.h>

#define REF_LEN 60
#define MAX_FRAMES 10

/* Deterministic reference string with repeated locality around pages 1..5. */
static const int reference[REF_LEN] = {
    1,2,3,4,1,2,3,4,5,1,2,3,4,5,1,
    2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,
    5,6,7,1,2,3,4,5,6,7,1,2,3,4,5,
    6,7,8,1,2,3,4,5,6,7,8,1,2,3,4
};

static int find_page(const int frames[], int count, int page)
{
    for (int i = 0; i < count; ++i)
        if (frames[i] == page) return i;
    return -1;
}

/* FIFO page-fault count for a fixed reference string and frame count. */
static int fifo_faults(int frame_count)
{
    int frames[MAX_FRAMES];
    int used = 0, next = 0, faults = 0;

    for (int i = 0; i < REF_LEN; ++i) {
        if (find_page(frames, used, reference[i]) >= 0)
            continue;

        ++faults;
        if (used < frame_count) {
            frames[used++] = reference[i];
        } else {
            frames[next] = reference[i];
            next = (next + 1) % frame_count;
        }
    }

    return faults;
}

int main(void)
{
    FILE *fp = fopen("frame_faults.csv", "w");
    if (fp == NULL) {
        perror("frame_faults.csv");
        return EXIT_FAILURE;
    }

    puts("FIFO page faults against frame count");
    puts("Frames,PageFaults");
    fprintf(fp, "Frames,PageFaults\n");

    for (int f = 1; f <= MAX_FRAMES; ++f) {
        int faults = fifo_faults(f);
        printf("%d,%d\n", f, faults);
        fprintf(fp, "%d,%d\n", f, faults);
    }

    fclose(fp);
    return 0;
}
