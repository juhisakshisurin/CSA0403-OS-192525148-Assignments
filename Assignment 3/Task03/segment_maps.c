#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

static int global_data = 42;

int main(int argc, char **argv)
{
    int hold = (argc > 1 && strcmp(argv[1], "--hold") == 0);
    int local_stack = 7;
    char *heap_data = malloc(4096);

    if (heap_data == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    strcpy(heap_data, "heap demonstration");

    printf("PID: %ld\n", (long)getpid());
    printf("Addresses: code=%p global/data=%p heap=%p stack=%p\n",
           (void *)&main, (void *)&global_data,
           (void *)heap_data, (void *)&local_stack);
    puts("\n/proc/self/maps (running process memory mappings):");

    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp == NULL) {
        perror("fopen /proc/self/maps");
        free(heap_data);
        return EXIT_FAILURE;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp) != NULL)
        fputs(line, stdout);

    fclose(fp);

    if (hold) {
        printf("\nHolding PID %ld for 20 seconds for /proc/<pid>/maps inspection.\n",
               (long)getpid());
        fflush(stdout);
        struct timespec delay = {20, 0};
        while (nanosleep(&delay, &delay) == -1) {
            /* Resume if interrupted. */
        }
    }

    free(heap_data);
    return 0;
}
