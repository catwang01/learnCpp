// handle_signals.cpp
// 本程序演示 signal 函数的用法
#include<signal.h>
#include<bits/sigset.h>
#include<stdio.h>
#include <unistd.h>

//typedef void (*sighandler_t)(int);
void handler(int signum)
{
    if(signum == SIGIO)
        printf("SIGIO signal: %d\n", signum);
    else if(signum == SIGUSR1)
        printf("SIGUSR1 signal: %d\n", signum);
    else
        printf("error\n");
}

int main()
{
    signal(SIGIO, handler);
    signal(SIGUSR1, handler);
    printf("I will handle SIGIO, SIGUSR1\n");
    while (1)
        sleep(10000);
    return 0;
}