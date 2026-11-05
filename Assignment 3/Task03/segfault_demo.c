#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        /* Deliberate invalid memory access in a child process only. */
        volatile int *invalid = (volatile int *)0;
        *invalid = 123;
        _exit(EXIT_SUCCESS);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
        printf("Child %ld terminated by SIGSEGV (signal %d).\n",
               (long)pid, WTERMSIG(status));
        puts("The OS detected an invalid virtual-memory access and "
             "delivered SIGSEGV; the parent process continued.");
        return EXIT_SUCCESS;
    }

    printf("Unexpected child status: 0x%x\n", status);
    return EXIT_FAILURE;
}
