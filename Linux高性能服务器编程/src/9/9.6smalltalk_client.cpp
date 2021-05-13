#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>

#define BUFFER_SIZE 64

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    struct sockaddr_in server_address;
    bzero( &server_address, sizeof( server_address ) );
    server_address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &server_address.sin_addr );
    server_address.sin_port = htons( port );

    int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sockfd >= 0 );
    if ( connect( sockfd, ( struct sockaddr* )&server_address, sizeof( server_address ) ) < 0 )
    {
        printf( "connection failed\n" );
        close( sockfd );
        return 1;
    }

    pollfd fds[2];
    fds[0].fd = 0; // 文件描述符为 0 表示标准输入
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = sockfd; // 表示 client socket
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;
    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    int ret = pipe( pipefd );
    assert( ret != -1 );

    while( 1 )
    {
        ret = poll( fds, 2, -1 ); // timeout == -1, poll 阻塞读取
        if( ret < 0 )
        {
            printf( "poll failure\n" );
            break;
        }

        // 当前客户端有两种数据要输出到屏幕上
        // 1. 当前客户端输入的数据，这个可以通过 scanf + printf 解决
        // 2. 其他客户端输入的数据，由于服务器将其他客户端输入的数据 send 到当前客户端，因此当前客户端只需要 recv + printf 就可以
        // 这份代码没有使用 scanf + printf 的方法处理第一种输入，而是通过 splice 将标准输入的数据copy到当前客户端的 sock 中
        // 这样，就可以用第二种方式处理第一种输入，相当于将两种不同的输入方式用同一种输出方式输出了出来
        // 至于这样做的意义除了可以统一之外，不知道还有没有别的？

        if( fds[1].revents & POLLRDHUP ) // server 关闭
        {
            printf( "server close the connection\n" );
            break;
        }
        else if( fds[1].revents & POLLIN ) // 分支2
        {
            memset( read_buf, '\0', BUFFER_SIZE );
            recv( fds[1].fd, read_buf, BUFFER_SIZE-1, 0 );
            printf( "%s\n", read_buf );
        }

        if( fds[0].revents & POLLIN ) // 文件描述符0（即 stdin 可读）
        {
            // 将标准输入直接写到 socket 上 零拷贝
            ret = splice( 0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );
            ret = splice( pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );
            //此时 sockfd 是可读的，会被分支2处理掉
        }
    }
    
    close( sockfd );
    return 0;
}