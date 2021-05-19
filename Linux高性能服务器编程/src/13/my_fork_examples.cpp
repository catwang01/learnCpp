#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <wait.h>

#define BUFF_SIZE 1024

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./%s string\n", basename(argv[0]));
        return 0;
    }
    const char* message = argv[1];

    int pipefd[2];
    assert(pipe(pipefd) != -1);
    int ret = 0;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 0;
    }
    else if (pid == 0) // Child process branch
    {
        char buff[BUFF_SIZE];
        memset(buff, 0, sizeof(buff));
        close(pipefd[1]); // close write end
        ret = read(pipefd[0], buff, sizeof(buff));
        printf("[Child] get message %d from parent: %s\n", ret, buff);
        _exit(0);
    }
    else // Parent process branch
    {
        close(pipefd[0]); // close read end
        printf("[Parent] send %d message to child %d: %s\n", strlen(message), pid, message);
        write(pipefd[1], message, strlen(message));
        wait(NULL);
        return 0;
    }

}