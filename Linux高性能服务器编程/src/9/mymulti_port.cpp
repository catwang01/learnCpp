// 本程序参考 9.8multi_port 但有较大不同
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define IP "127.0.0.1"
#define PORT 5000
#define MAX_CONNECTION 5
#define  MAX_EVENTS MAX_CONNECTION + 2
#define BUFF_SIZE 1024

// epfd 内核事件表的文件描述符
// 要监听的文件描述符
// 要监听的文件描述符所对应的事件
int add_ctl(int epfd, int fd, int events)
{
    epoll_event event;
    event.events = events;
    event.data.fd = fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);  // 获取旧的文件描述符的状态
    int new_option = old_option | O_NONBLOCK; // 设置非阻塞模式
    fcntl(fd, F_SETFL, new_option);
    return old_option; // 返回旧的状态，以便恢复
}

int main()
{
    int listenfd_tcp = socket(AF_INET, SOCK_STREAM, 0);
    int sock_udp = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    bind(listenfd_tcp, (sockaddr*)&serv_addr, sizeof(serv_addr));
    bind(sock_udp, (sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd_tcp, 5);
    sockaddr_in udp_client_addr;
    socklen_t udp_client_addr_len = sizeof(udp_client_addr);

    printf("Start a tcp server at %s:%d...\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
    printf("Start a udp server at %s:%d...\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

    int epfd = epoll_create(MAX_EVENTS); // 创建内核事件表

    add_ctl(epfd, listenfd_tcp, EPOLLIN | EPOLLERR);
    add_ctl(epfd, sock_udp, EPOLLIN | EPOLLERR);

    epoll_event events[MAX_EVENTS];
    int sockfd = -1;
    int n_connect = 0;
    char buff[BUFF_SIZE];
    memset(buff, 0, sizeof(buff));
    while(true)
    {
        int n_ok = epoll_wait(epfd, events, MAX_EVENTS, 5);
        for (int i=0; i<n_ok; i++)
        {
            sockfd = events[i].data.fd;
            if (sockfd == listenfd_tcp) // 如果是 tcp 监听连接
            {
                if (events[i].events & EPOLLERR)
                {
                    perror("Unexpected error");
                    close(sockfd);
                    break;
                }
                else if (events[i].events & EPOLLIN)
                {
                    sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(sockaddr_in);
                    int connfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
                    if (connfd == -1)
                    {
                        perror("Listenfd exits!");
                        break;
                    }
                    if (n_connect >= MAX_CONNECTION) 
                    {
                        const char* info = "Reach max connection!";
                        send(connfd, info, strlen(info), 0);
                        close(connfd);
                    }
                    else
                    {
                        setnonblocking(connfd);
                        add_ctl(epfd, connfd, EPOLLIN |EPOLLERR);
                        n_connect++;
                        printf("A new tcp connection! Current connections: %d\n", n_connect);
                    }
                }
            }
            else if (sockfd == sock_udp) // 如果是 udp 连接
            {
                if (events[i].events & EPOLLERR)// EPOLLERR
                {
                    close(sockfd);
                    break;
                }
                else if (events[i].events & EPOLLIN) // 可读
                {
                    int ret = recvfrom(sockfd, buff, sizeof(buff), 0, (sockaddr*)&udp_client_addr, &udp_client_addr_len);
                    if (ret > 0)
                    {
                        sendto(sockfd, buff, sizeof(buff), 0, (sockaddr*)&udp_client_addr, udp_client_addr_len);
                    }
                    else {
                        perror("Unexpected error");
                    }
                    memset(buff, 0, sizeof(buff));
                }
            }
            else // tcp 连接 socket
            {
                if (events[i].events & EPOLLERR)// EPOLLRDHUP
                {
                    sockaddr_in tcp_client_addr;
                    socklen_t tcp_client_addr_len = sizeof(tcp_client_addr);
                    getpeername(events[i].data.fd, (sockaddr*)&tcp_client_addr, &tcp_client_addr_len);
                    printf("Unexpected error occurred for %s:%d\n", inet_ntoa(tcp_client_addr.sin_addr), ntohs(tcp_client_addr.sin_port));
                    close(sockfd);
                    n_connect--;
                    printf("Current connection: %d\n", n_connect);
                }
                else if (events[i].events & EPOLLIN) // 可读
                {
                    int ret = recv(sockfd, buff, sizeof(buff), 0);
                    if (ret > 0)
                    {
                        send(sockfd, buff, strlen(buff), 0);
                    }
                    else if (ret == 0)
                    {
                        sockaddr_in tcp_client_addr;
                        socklen_t tcp_client_addr_len = sizeof(tcp_client_addr);
                        getpeername(events[i].data.fd, (sockaddr*)&tcp_client_addr, &tcp_client_addr_len);
                        printf("One client exits %s:%d\n", inet_ntoa(tcp_client_addr.sin_addr), ntohs(tcp_client_addr.sin_port));
                        close(sockfd);
                        n_connect--;
                        printf("Current connection: %d\n", n_connect);
                    }
                    else
                    {
                        perror("Unexpected error");
                    }
                    memset(buff, 0, sizeof(buff));
                }
            }
        }
    }
}