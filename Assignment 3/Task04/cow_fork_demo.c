#define _GNU_SOURCE
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

static void show_rollup(const char *label)
{
    FILE *fp = fopen("/proc/self/smaps_rollup", "r");
    if (fp == NULL) {
        perror("open /proc/self/smaps_rollup");
        return;
    }

    printf("%s (PID %ld):\n", label, (long)getpid());

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "Rss:", 4) == 0 ||
            strncmp(line, "Pss:", 4) == 0 ||
            strncmp(line, "Shared_Clean:", 13) == 0 ||
            strncmp(line, "Shared_Dirty:", 13) == 0 ||
            strncmp(line, "Private_Clean:", 14) == 0 ||
            strncmp(line, "Private_Dirty:", 14) == 0) {
            fputs(line, stdout);
        }
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    size_t mb = 64;
    if (argc == 2) {
        char *end = NULL;
        unsigned long long parsed = strtoull(argv[1], &end, 10);
        if (end == argv[1] || *end != '\0' || parsed == 0 ||
            parsed > (unsigned long long)(SIZE_MAX / (1024U * 1024U))) {
            fprintf(stderr, "Usage: %s [memory_size_MB]\n", argv[0]);
            return EXIT_FAILURE;
        }
        mb = (size_t)parsed;
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [memory_size_MB]\n", argv[0]);
        return EXIT_FAILURE;
    }

    long page_long = sysconf(_SC_PAGESIZE);
    if (page_long <= 0) {
        fprintf(stderr, "Could not obtain page size.\n");
        return EXIT_FAILURE;
    }
    size_t page_size = (size_t)page_long;
    size_t bytes = mb * 1024U * 1024U;

    unsigned char *memory = mmap(NULL, bytes, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < bytes; i += page_size)
        memory[i] = (unsigned char)((i / page_size) & 0xffU);

    printf("Copy-on-write demonstration using %zu MiB.\n", mb);
    show_rollup("Parent before fork");

    fflush(stdout);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        munmap(memory, bytes);
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        show_rollup("Child after fork, before writes");
        fflush(stdout);

        /* Writing one byte per page causes private COW copies as needed. */
        for (size_t i = 0; i < bytes; i += page_size)
            memory[i] ^= 0x5aU;

        show_rollup("Child after writing every page");
        fflush(stdout);
        munmap(memory, bytes);
        _exit(EXIT_SUCCESS);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        munmap(memory, bytes);
        return EXIT_FAILURE;
    }

    show_rollup("Parent after child completed");
    munmap(memory, bytes);

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "COW child did not exit successfully.\n");
        return EXIT_FAILURE;
    }

    return 0;
}
