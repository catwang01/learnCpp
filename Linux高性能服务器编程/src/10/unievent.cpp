#include <iostream>
#include <string.h> // for memset basename
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h> // for atoi
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

int pipefd[2];

void signal_handler(int sig)
{
    int save_no = errno;
    printf("handle signal %d\n", sig);
    int ret = send(pipefd[1], (char*)&sig, 1, 0);
    if (ret > 0)
    {
        printf("send %d bytes\n", ret);
    }
    errno = save_no;
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);  // 获取旧的文件描述符的状态
    int new_option = old_option | O_NONBLOCK; // 设置非阻塞模式
    fcntl(fd, F_SETFL, new_option);
    return old_option; // 返回旧的状态，以便恢复
}

void register_signal(int sig)
{
    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    int ret = sigaction(sig, &sa, NULL);
    assert (ret != -1);
}

void addfd(int epollfd, int fd)
{
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLERR;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        printf("Usage: ./%s IP PORT\n", basename(argv[0]));
        return -1;
    }
    const char* ip = argv[1];
    const int port = atoi(argv[2]);
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = -1;
    ret = bind(listenfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    assert (ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    // ret = pipe(pipefd);
    ret = socketpair( PF_UNIX, SOCK_STREAM, 0, pipefd ); // 创建双向管道
    assert(ret != -1);

    int epollfd = epoll_create(5);
    addfd(epollfd, listenfd);
    addfd(epollfd, pipefd[0]);

    epoll_event events[5];
    memset(events, 0, sizeof(events));
    int n_conn = 0;
    char buff[1024];
    bzero(buff, sizeof(buff));

    register_signal(SIGHUP);
    register_signal(SIGTERM);
    register_signal(SIGQUIT);
    register_signal(SIGINT);
    bool stop_server = false;

    while (!stop_server)
    {
        ret = epoll_wait(epollfd, events, 5, -1);
        printf("%d %d\n", errno, ret);
        if (ret < 0 && errno != EINTR)
        {
            break;
        }
        for (int i=0; i<ret; i++)
        {
            int socket = events[i].data.fd;
            if (socket == listenfd && events[i].events & EPOLLIN) // 监听socket可读，说明有新的连接
            {
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int connfd = accept(listenfd, (sockaddr*)&client_addr, &client_addr_len);
                if (connfd > 0)
                {
                    addfd(epollfd, connfd);
                    n_conn++;
                    printf("Clients %s:%d is connected to server. Number of currect connections: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), n_conn);
                }
            }
            else 
            {
                if (socket == pipefd[0] && events[i].events & EPOLLIN)
                {
                    ret = recv(pipefd[0], buff, sizeof(buff), 0); // ret 表示接受到到信号的个数
                    if (ret > 0)
                    {
                        for (int i=0; i<ret; i++)
                        {
                           switch(buff[i]) 
                           {
                                printf("I caught a signal: %c\n", buff[i]);
                           }
                        }
                        bzero(buff, sizeof(buff));
                    }
                    else if (ret == -1 && errno != EAGAIN)
                    {
                    }
                    else {
                        printf("Pipeline closed");
                    }
                }
            }
        }
    }
    close(listenfd);
    return 0;
}