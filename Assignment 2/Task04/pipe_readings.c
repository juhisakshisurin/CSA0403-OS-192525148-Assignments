#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Parent (sensor reader) sends readings to the child (analyser) through a pipe. */
int main(void)
{
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        close(fd[1]); /* child only reads */

        int r, count = 0, sum = 0, max = 0;
        ssize_t n;

        /* TODO 1 completed: read integers until EOF (read returns 0). */
        while ((n = read(fd[0], &r, sizeof(r))) > 0) {
            if (n != (ssize_t)sizeof(r)) {
                fprintf(stderr, "Incomplete integer received from pipe.\n");
                close(fd[0]);
                _exit(EXIT_FAILURE);
            }

            count++;
            sum += r;
            if (count == 1 || r > max) {
                max = r;
            }
        }

        if (n < 0) {
            perror("read");
            close(fd[0]);
            _exit(EXIT_FAILURE);
        }

        printf("analyser: %d readings, average %.1f, max %d\n",
               count, count ? (double)sum / count : 0.0, max);

        close(fd[0]);
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    close(fd[0]); /* parent only writes */

    int readings[] = {31, 33, 36, 34, 41, 38};

    for (size_t i = 0; i < sizeof(readings) / sizeof(readings[0]); i++) {
        ssize_t written = write(fd[1], &readings[i], sizeof(readings[i]));
        if (written != (ssize_t)sizeof(readings[i])) {
            if (written < 0) perror("write");
            else fprintf(stderr, "Short write to pipe.\n");
            close(fd[1]);
            (void)waitpid(pid, NULL, 0);
            return EXIT_FAILURE;
        }
    }

    close(fd[1]); /* signals end-of-data (EOF) to the child */

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "Analyser child did not complete successfully.\n");
        return EXIT_FAILURE;
    }

    printf("reader: done\n");
    return EXIT_SUCCESS;
}
