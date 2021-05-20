// This code is example code from
// pipe(2) - Linux manual page
// https://man7.org/linux/man-pages/man2/pipe.2.html#EXAMPLES
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) // child branch
    {                     /* Child reads from pipe */
        close(pipefd[1]); /* Child doesn't use write end, close it */

        while (read(pipefd[0], &buf, 1) > 0) // reads a char from pipefd[0] at a time
        {
            write(STDOUT_FILENO, &buf, 1);  // echoes to the stdout
        }
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS); // use _exit in child process
    }
    else // parent branch
    {                     /* Parent writes argv[1] to pipe */
        close(pipefd[0]); /* Parent doesn't use read end, close it */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]); /* Reader will see EOF */
        wait(NULL);       /* Wait for child */
        exit(EXIT_SUCCESS); // use exit in Parent process
    }
}