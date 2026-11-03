/*
 * Name: Juhi Surin
 * Register Number: 192525148
 * Course: CSA0403 - Operating Systems
 * Assignment: Assignment 2 - Task 1
 * Date: 03-11-2026
 * Under the Supervision of: Dr. Vivek Balaji
 *
 * Program: Process Creation using fork()
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHILD_COUNT 3
#define HOLD_SECONDS 8

int main(void)
{
    pid_t children[CHILD_COUNT];
    int created = 0;

    printf("Parent process PID: %ld\n", (long)getpid());
    fflush(stdout);

    // Create child processes
    for (int i = 0; i < CHILD_COUNT; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            break;
        }

        if (pid == 0)
        {
            // Child process
            printf("Child %d started: PID=%ld, PPID=%ld\n",
                   i + 1,
                   (long)getpid(),
                   (long)getppid());

            fflush(stdout);

            // Keep child alive for process observation
            sleep(HOLD_SECONDS);

            printf("Child %d (PID=%ld) terminating.\n",
                   i + 1,
                   (long)getpid());

            fflush(stdout);

            _exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            children[created++] = pid;

            printf("Parent created child %d with PID=%ld\n",
                   created,
                   (long)pid);

            fflush(stdout);
        }
    }

    // Wait for all child processes
    for (int i = 0; i < created; i++)
    {
        int status = 0;

        pid_t result = waitpid(children[i], &status, 0);

        if (result == -1)
        {
            perror("waitpid");
            continue;
        }

        if (WIFEXITED(status))
        {
            printf("Parent reaped child PID=%ld, exit status=%d\n",
                   (long)result,
                   WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Parent reaped child PID=%ld, signal=%d\n",
                   (long)result,
                   WTERMSIG(status));
        }
    }

    printf("Parent process completed.\n");

    return (created == CHILD_COUNT) ? EXIT_SUCCESS : EXIT_FAILURE;
}
