#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
static int pipefd[2]; // 管道

int setnonblocking( int fd ) // 设置文件描述符为非阻塞
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd ) // 添加到 epoll 的事件内核表
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

void sig_handler( int sig ) // 信号处理函数
{
    int save_errno = errno; // 保存 errno
    int msg = sig;
    send( pipefd[1], ( char* )&msg, 1, 0 ); // 将信号发送给管道，通知主循环
                                            // 这个是关键
    errno = save_errno;
}

void addsig( int sig )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART; // SA_RESTART 表示重新调用被该信号终止的系统调用
                                // Q(ed): what if we omit this SA_RESTART? 
                                // A: Some function like recv, send may be interrupted by the system signal 
                                // and be not atomatically restarted
    sigfillset( &sa.sa_mask );  // A(ed):为什么将所有信号都 block了？
                                // Q: sa.sa_mask specifies which signals to block during the execution of the signal handler,
                                // instead of during the whole lifetime of the program.
    assert( sigaction( sig, &sa, NULL ) != -1 );
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

    struct sockaddr_in address; // 定义服务器地址
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 ); // 监听 socket
    assert( listenfd >= 0 );

    //int nReuseAddr = 1;
    //setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &nReuseAddr, sizeof( nReuseAddr ) );
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    if( ret == -1 )
    {
        printf( "errno is %d\n", errno );
        return 1;
    }

    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd ); // 将监听sock对应的事件添加到内核事件表中

    ret = socketpair( PF_UNIX, SOCK_STREAM, 0, pipefd );  // 创建双向管道
                                                          // TODO(ed)：为什么这里要一个双向管道？貌似没有用到呀？
    // 实验了一下，单向管道有问题，不能达到效果
    // ret = pipe(pipefd);
    assert( ret != -1 );
    setnonblocking( pipefd[1] ); // 写端设置为非阻塞
    addfd( epollfd, pipefd[0] ); // 读端监听 EPOLLIN 事件

    // add all the interesting signals here
    addsig( SIGHUP ); 
    addsig( SIGQUIT ); // `ctrl + \`
    addsig( SIGCHLD ); // Child stopped or terminated
    addsig( SIGTERM ); // Termination signal
    addsig( SIGINT ); // Interrupt from keyboard ctrl + c

    bool stop_server = false; // 由于停止循环的逻辑在一个二重循环内层，不能直接 break，因此用指示变量

    while( !stop_server )
    {
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );   // timeout == -1, epoll_wait will block indefinitely
        if ( ( number < 0 ) && ( errno != EINTR ) )                         // when errno == EINTE，epoll_wait was interrupted by a signal
                                                            // SOLVED(ed): 由于之前设置了 SA_RESTART，errno!= EINTR 这个判断是不是可以不用？
                                                            // No. Not all functions are restartable. epoll_wait will always return -1 with errno regardless of the use of SA_RESTART
        {
            printf( "epoll failure\n" );
            break;
        }
    
        for ( int i = 0; i < number; i++ )
        {
            int sockfd = events[i].data.fd;
            if( sockfd == listenfd ) // 如果是监听socket，说明有连接
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                addfd( epollfd, connfd );
            } // 这里为了简化代码，没有做错误处理
            else if( ( sockfd == pipefd[0] ) && ( events[i].events & EPOLLIN ) )
            {
                char signals[1024]; // 用来接受信号。最多接受 1024 个信号
                ret = recv( pipefd[0], signals, sizeof( signals ), 0 );
                if( ret == -1 ) // TODO(ed): 这里应该有两种情况。一种是非阻塞返回 ret==-1，一种是未知错误返回 -1，是不是应该分开讨论？
                {
                    continue;
                }
                else if( ret == 0 ) // TODO(ed):什么时候会出现？管道不可读？
                {
                    continue;
                }
                else
                {
                    for( int i = 0; i < ret; ++i )
                    {
                        printf( "I caugh the signal %d\n", signals[i] );
                        switch( signals[i] )
                        {
                            case SIGCHLD:
                            case SIGHUP:
                            {
                                continue;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                                stop_server = true;
                            }
                        }
                    }
                }
            }
            else 
            {
                // 这里处理客户端发送的消息
            }
        }
    }

    printf( "close fds\n" );
    close( listenfd );
    close( pipefd[1] );
    close( pipefd[0] );
    // TODO(ed)：貌似没有关闭客户端连接socket？
    return 0;
}
