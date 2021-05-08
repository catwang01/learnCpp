#define _GNU_SOURCE 1 // for POLLDRHUP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>

#define USER_LIMIT 5
#define BUFFER_SIZE 64
#define FD_LIMIT 65535

struct client_data // 定义结构体来保持客户端数据
{
    sockaddr_in address;
    char* write_buf;    // 注意是一个指针，而非一个 char write_buf[BUFFER_SIZE]
                        // 这个设计是根据聊天室程序的特点决定的
                        // 要写入某个客户端的数据，实际上是从其他客户端读到的。
                        // 因此不需要一个独立的写缓冲区，只需要一个指针，只想其他客户端的读缓冲区即可
    char buf[ BUFFER_SIZE ];
};

int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 ); // 服务端socket
    assert( listenfd >= 0 );

    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );

    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    client_data* users = new client_data[FD_LIMIT];
    pollfd fds[USER_LIMIT+1]; // 大小为 USER_LIMIT+1，0位置放server sock，1 ~ USER_LIMIT位置放 client 的文件描述符
    int user_counter = 0;
    for( int i = 1; i <= USER_LIMIT; ++i ) // 初始化
    {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = listenfd;
    fds[0].events = POLLIN | POLLERR; // 监听 socket 只接受两个事件 1. 可读 2. 发生错误
    fds[0].revents = 0;

    while( 1 )
    {
        ret = poll( fds, user_counter+1, -1 ); // timeout==-1, poll 阻塞
        if ( ret < 0 )
        {
            printf( "poll failure\n" );
            break;
        }
    
        for( int i = 0; i < user_counter+1; ++i )
        {
            if( ( fds[i].fd == listenfd ) && ( fds[i].revents & POLLIN ) )  // listenfd 可读，说明有客户端连接，此时要做下面几件事
                                                                            // 1. 使用 accept 获取一个 connfd，表示客户端的连接
                                                                            // 2. 看 client 的数量是否达到 USER_LIMIT，如果达到了，那么就告知 connfd，并关闭 connfd
                                                                            // 3. 如果没有达到，那么要将 connfd 放到 users 中，并将 connfd 设置为非阻塞
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                if ( connfd < 0 ) // 建立失败
                {
                    printf( "errno is: %d\n", errno );
                    continue;
                }
                if( user_counter >= USER_LIMIT ) // 用户数超过限制，向当前 client 的 socket 发送 too many users 提示
                {
                    const char* info = "too many users\n";
                    printf( "%s", info );
                    send( connfd, info, strlen( info ), 0 );
                    close( connfd );
                    continue;
                }
                user_counter++;
                users[connfd].address = client_address;
                // 这里需要设置为非阻塞
                setnonblocking( connfd );
                fds[user_counter].fd = connfd;
                // 监听三种事件 1. 可读 2. 关闭连接 3. 错误
                // 为什么这里不监听可写？因为还没有数据（即还没有客户端在聊天室内发言），此时不需要对其他客户端写入数据，因此也就不需要监听可写事件
                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_counter].revents = 0;
                printf( "comes a new user, now have %d users\n", user_counter );
            }
            else if( fds[i].revents & POLLERR ) // 如果报错了，清除错误后继续
            {
                printf( "get an error from %d\n", fds[i].fd );
                char errors[ 100 ];
                memset( errors, '\0', 100 );
                socklen_t length = sizeof( errors );
                if( getsockopt( fds[i].fd, SOL_SOCKET, SO_ERROR, &errors, &length ) < 0 ) // 清除错误
                {
                    printf( "get socket option failed\n" );
                }
                continue;
            }
            else if( fds[i].revents & POLLRDHUP ) // 如果 i 位置的连接关闭
            {
                // 下面的逻辑实际上就是在说如果 fds[i] 对应的 socket 连接断开了，就释放对应的资源
                // 只是因为没有用到的高级数据结构而只使用数组，因此比较麻烦

                users[fds[i].fd] = users[fds[user_counter].fd]; // 将 i 位置的 socket 释放，并将最后一个连接放到 i 的位置
                close( fds[i].fd );
                fds[i] = fds[user_counter];
                i--; // i--，因为已经将 fds[user_counter] 放到  fds[i] 的位置。因此需要退回 i，这样才能保证新的 fds[i] 被处理
                user_counter--;
                printf( "a client left\n" );
            }
            else if( fds[i].revents & POLLIN ) // client 可读 说明有客户端发请求到服务器
            {
                int connfd = fds[i].fd;
                memset( users[connfd].buf, '\0', BUFFER_SIZE );
                ret = recv( connfd, users[connfd].buf, BUFFER_SIZE-1, 0 );
                printf( "get %d bytes of client data %s from %d\n", ret, users[connfd].buf, connfd );
                if( ret < 0 ) // 读取失败
                {
                    if( errno != EAGAIN )   // errno == EAGAIN 表示在非阻塞状态，还不能读
                                            // TODO(ed): 这个状态应该不会有吧？poll 的结果都是可读了，还会返回 errno == EAGAIN 的情况吗？
                    {
                        close( connfd );
                        users[fds[i].fd] = users[fds[user_counter].fd];
                        fds[i] = fds[user_counter];
                        i--;
                        user_counter--;
                    }
                    // errno == EAGAIN 不做处理
                }
                else if( ret == 0 ) // recv返回0，说明对方已经关闭连接
                                    // 此时由POLLRDHUP事件处理，因此这里不做处理
                {
                    printf( "code should not come to here\n" );
                }
                else // 读取成功 
                {
                    for( int j = 1; j <= user_counter; ++j ) // 修改除了当前发消息的客户端（即connfd）外所有的 client 让 poll 监听 POLLOUT 事件
                    {
                        if( fds[j].fd == connfd ) 
                        {
                            continue;
                        }
                        
                        fds[j].events |= ~POLLIN;
                        fds[j].events |= POLLOUT;
                        users[fds[j].fd].write_buf = users[connfd].buf; // 其他 client 的 write_buf 指针指向 connfd 的 buf 区域
                    }
                }
            }
            else if( fds[i].revents & POLLOUT ) // 数据可写
            {
                int connfd = fds[i].fd;
                if( ! users[connfd].write_buf ) // 没有数据可写
                {
                    continue;
                }
                ret = send( connfd, users[connfd].write_buf, strlen( users[connfd].write_buf ), 0 );
                // TODO(ed)：这里没有判断写的结果如何。直接就把 write_buf 清除了，这个有问题吧
                users[connfd].write_buf = NULL;
                fds[i].events |= ~POLLOUT;  // 数据写完之后
                                            // 改为监听 POLLIN 事件，而非 POLLOUT 事件
                fds[i].events |= POLLIN;
            }
        }
    }

    delete [] users;
    close( listenfd );
    return 0;
}