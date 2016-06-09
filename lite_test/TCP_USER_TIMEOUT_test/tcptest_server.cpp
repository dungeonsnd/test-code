
/*
 TCP_USER_TIMEOUT (since Linux 2.6.37)
              This option takes an unsigned int as an argument.  When the
              value is greater than 0, it specifies the maximum amount of
              time in milliseconds that transmitted data may remain
              unacknowledged before TCP will forcibly close the
              corresponding connection and return ETIMEDOUT to the
              application.  If the option value is specified as 0, TCP will
              to use the system default.

              Increasing user timeouts allows a TCP connection to survive
              extended periods without end-to-end connectivity.  Decreasing
              user timeouts allows applications to "fail fast", if so
              desired.  Otherwise, failure may take up to 20 minutes with
              the current system defaults in a normal WAN environment.

              This option can be set during any state of a TCP connection,
              but is only effective during the synchronized states of a
              connection (ESTABLISHED, FIN-WAIT-1, FIN-WAIT-2, CLOSE-WAIT,
              CLOSING, and LAST-ACK).  Moreover, when used with the TCP
              keepalive (SO_KEEPALIVE) option, TCP_USER_TIMEOUT will
              override keepalive to determine when to close a connection due
              to keepalive failure.

              The option has no effect on when TCP retransmits a packet, nor
              when a keepalive probe is sent.

              This option, like many others, will be inherited by the socket
              returned by accept(2), if it was set on the listening socket.

              Further details on the user timeout feature can be found in
              RFC 793 and RFC 5482 ("TCP User Timeout Option").
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <arpa/inet.h>

#define TCP_USER_TIMEOUT       18      /* How long for loss retry before timeout */

#define ENABLE_USER_TIMEOUT 1

#define MAXBUF 1024
#define MAXEPOLLSIZE 10000

int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
}

int set_keepalive(int fd)
{
    int rt =0;
    do{
        int keepalive = 1; // 开启keepalive属性
        int keepidle = 60; // 如该连接在多少秒内没有任何数据往来,则进行探测
        int keepinterval = 12; // 探测时发包的时间间隔为多少秒
        int keepcount = 2; // 探测尝试的次数.如果第1次探测包就收到响应了,则后面的不再发.
        rt =setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
        if(0!=rt)
            break;
        rt =setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
        if(0!=rt)
            break;
        rt =setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
        if(0!=rt)
            break;
        rt =setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));
        if(0!=rt)
            break;
    }while(0);
    return rt;
}

int set_tcp_user_timeout(int fd)
{
#if ENABLE_USER_TIMEOUT
    int tcp_timeout =24*1000; // seconds before aborting a write()
    return setsockopt(fd, SOL_TCP, TCP_USER_TIMEOUT, &tcp_timeout, sizeof(int));
#else
	return 0;
#endif	
}


int handle_message(int clientfd)
{
    char buf[MAXBUF + 1];
    bzero(buf, MAXBUF + 1);
    ssize_t n = recv(clientfd, buf, MAXBUF, 0);
    if (n > 0)
    {
        printf("==>  fd{%d} R: %s len=%d\n",clientfd, buf, int(n));
		ssize_t sn =send(clientfd, buf, n, 0);
		buf[sn]='\0';
		printf("<==  fd{%d} S: %s len=%d\n",clientfd, buf, int(sn));		
    }
    else if(n==0)
    {
        printf("XXX  Remote client has closed! recv return n=%d\n",int(n));
        close(clientfd);
    }
    else
    {
        printf("     Failed recved! errno=%d,%s\n",errno, strerror(errno));
        close(clientfd);
    }
    return n;
}


int main(int argc, char **argv)
{
	if(argc<2)
	{
		fprintf(stderr,"Usage:\n %s <port>\n",argv[0]);
		return 1;
	}
    int serverPort =atoi(argv[1]);
	if(serverPort<1)
	{
		fprintf(stderr,"serverPort{%d} error\n",serverPort);
		return 1;
	}
    
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    
    int listenerfd = socket(PF_INET, SOCK_STREAM, 0);
    if (-1==listenerfd)
    {
        printf("ERROR socket ,errno=%d,rt=%d,%s \n",errno,listenerfd,strerror(errno));
        return 0;
    }

	int opt=SO_REUSEADDR;
    setsockopt(listenerfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	
    setnonblocking(listenerfd);
    
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(serverPort);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenerfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) 
    {
        printf("ERROR bind ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    } 
    if (listen(listenerfd, 128) == -1) 
    {
        printf("ERROR listen ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }

    int efd = epoll_create(MAXEPOLLSIZE);
    socklen_t len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenerfd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listenerfd, &ev) < 0) 
    {
        printf("ERROR epoll_ctl ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }
    
    int maxevents = 1;
    while (maxevents>0) 
    {
        int n = epoll_wait(efd, events, maxevents, -1);
		printf("      epoll_wait returned %d \n",n);
        if (n == -1)
        {
            printf("ERROR epoll_wait ,errno=%d,%s \n",errno,strerror(errno));
            return 0;
        }
     
        for (int i = 0; i < n; ++i)
        {
            if (events[i].data.fd == listenerfd) 
            {
                struct sockaddr_in their_addr;
                int clientfd = accept(listenerfd, (struct sockaddr *) &their_addr,&len);
                if (clientfd < 0) 
                {
                    printf("ERROR accept ,errno=%d,%s \n",errno,strerror(errno));
                    continue;
                } 
                else
                {
                    printf("     Connected from %s:%d， client socket:%d\n",
                            inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), clientfd);
                }
                setnonblocking(clientfd);
                
                if(0!=set_keepalive(clientfd))
                {
                    printf("ERROR set_keepalive ,errno=%d,%s \n",errno,strerror(errno));
                    continue;
                }
                if(0!=set_tcp_user_timeout(clientfd))
                {
                    printf("ERROR set_tcp_user_timeout ,errno=%d,%s \n",errno,strerror(errno));
                    continue;
                }
                
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientfd;
                if (epoll_ctl(efd, EPOLL_CTL_ADD, clientfd, &ev) < 0)
                {
                    printf("ERROR epoll_ctl ,errno=%d,%s \n",errno,strerror(errno));
                    return -1;
                }
                maxevents++;
            } 
            else
            {
                int rt = handle_message(events[i].data.fd);
                if (rt < 1 && errno != 11)
                {
                    epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd,&ev);
                    maxevents--;
                }
            }
        }
    }
    close(listenerfd);
    return 0;
}
