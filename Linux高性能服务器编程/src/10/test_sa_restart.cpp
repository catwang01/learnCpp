// test_sa_restart.cpp
// 本程序测试 sa_restart 是如何工作的。
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <unistd.h>

void sig_handler(int signum)
{
    printf("in handler\n");
    sleep(1);
    printf("handler return\n");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s [1|2]\n", basename(argv[0]));
        return -1;
    }
    int version = atoi(argv[1]);
    char buf[100];
    int ret;
    struct sigaction action;

    action.sa_handler = sig_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    /* 版本1:不设置SA_RESTART属性
     * 版本2:设置SA_RESTART属性 */
    if (version==2) action.sa_flags |= SA_RESTART;

    sigaction(SIGINT, &action, NULL);
    bzero(buf, 100);

    ret = read(0, buf, 100); // fd = 0 表示从标准输入中读取
    if (ret == -1) {
        perror("read");
    }

    printf("read %d bytes %s\n", ret, buf);
    return 0;
}