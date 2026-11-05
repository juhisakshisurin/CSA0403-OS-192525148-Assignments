#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void)
{
    pid_t pid;
    int status;
    printf("shell: running job 1\n");
    fflush(stdout);   /* Empty the buffer before fork() */
    // TODO 1: Create a child process using fork()
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        // Replace child process with expr command
        execlp("expr", "expr", "6", "*", "7", (char *)NULL);

        // Execute only if execlp() fails
        perror("execlp");
        _exit(127);
    }
    // Parent waits for child and obtains its exit status
    waitpid(pid, &status, 0);
    printf("job 1 exit status %d\n", WEXITSTATUS(status));
    printf("shell: running job 2\n");
    fflush(stdout);
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0)
        _exit(3);   /* Child exits with code 3 */
    waitpid(pid, &status, 0);
    printf("job 2 exit status %d\n", WEXITSTATUS(status));
    return 0;
}
