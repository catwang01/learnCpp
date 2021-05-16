#define TIMEOUT 5000

int timeout = TIMEOUT;
time_t start = time( NULL );
time_t end = time( NULL );
while( 1 )
{
    printf( "the timeout is now %d mill-seconds\n", timeout );
    start = time( NULL );
    int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, timeout );
    if( ( number < 0 ) && ( errno != EINTR ) )
    {
        printf( "epoll failure\n" );
        break;
    }
    if( number == 0 ) // epoll_wait returns 0 means timeout
    {
        // timeout
        // here may toggle callback function
        timeout = TIMEOUT; // reset timeout param after handling timed task 
        continue;
    }

    end = time( NULL ); // TODO(ed): why compute end time here(before handling connections)?
                        // why not compute end time after handling connections
    timeout -= ( end - start ) * 1000;
    if( timeout <= 0 ) // if timeout == 0, means  while the file descriptor is ready, the timeout period has also reached
    {
        // timeout
        // here may toggle callback function
        timeout = TIMEOUT; // reset timeout param after handling timed task
    }

    // handle connections
}