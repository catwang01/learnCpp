// 本程序测试 sigprocmask 的使用
#include <stdio.h>
#include <signal.h>
#include <unistd.h> // for sleep

int main()
{
    sigset_t old_set, new_set;

    sigaddset(&new_set, SIGINT);    // 屏蔽 ctrl + C
    sigaddset(&new_set, SIGQUIT);   // 屏蔽 `ctrl + \`
    sigaddset(&new_set, SIGTSTP);   // 屏蔽 ctrl + Z
    sigaddset(&new_set, SIGTERM);   // 屏蔽 kill 
    sigaddset(&new_set, SIGKILL);   // 屏蔽 kill -9 
                                    // 不能成功，因为 SIGKILL 不能屏蔽
    sigprocmask(SIG_BLOCK, &new_set, &old_set);
    while (1)
    {
        sleep(1000);
    }
    return 0;
}