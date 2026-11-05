#define _POSIX_C_SOURCE 200809L
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

extern uint64_t bank_transform(uint64_t value);
extern const char *bankcrypto_version(void);

static void show_relevant_maps(void)
{
    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp == NULL) {
        perror("fopen /proc/self/maps");
        return;
    }

    char line[512];
    puts("Relevant /proc/self/maps entries:");
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "bank_app") != NULL ||
            strstr(line, "libbankcrypto") != NULL ||
            strstr(line, "libc.so") != NULL ||
            strstr(line, "ld-linux") != NULL) {
            fputs(line, stdout);
        }
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    int hold = (argc > 1 && strcmp(argv[1], "--hold") == 0);

    printf("PID: %ld\n", (long)getpid());
    printf("Linked library version: %s\n", bankcrypto_version());
    printf("Demo transformation (not real cryptography): 0x%016" PRIx64 "\n",
           bank_transform(UINT64_C(0x1122334455667788)));

    show_relevant_maps();
    fflush(stdout);

    if (hold) {
        printf("Holding for 20 seconds so /proc/<pid> can be inspected...\n");
        fflush(stdout);
        struct timespec delay = {20, 0};
        while (nanosleep(&delay, &delay) == -1) {
            /* Resume after a signal if interrupted. */
        }
    }

    return EXIT_SUCCESS;
}
