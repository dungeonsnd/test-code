#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sys/epoll.h>
#include <errno.h>

#define MAXEPOLLSIZE 10000


int send_message(int clientfd,const std::string & buf)
{
    int n = send(clientfd, &buf[0], buf.size(),0);    
    if((n < 0)||(n == 0))
    {
        printf("send() failure!\n");
        return -1;
    }
    printf("<==  fd{%d} S: %s len=%d\n",clientfd, buf.c_str(), int(n));
    return n;
}    

int recv_message(int clientfd)
{
    std::string bufRecv(2048,'\0');
    int n = recv(clientfd, &bufRecv[0], bufRecv.size(),0);  
    if (n > 0)
    {
        printf("==>  fd{%d} R: %s len=%d\n",clientfd, bufRecv.c_str(), int(n));	
    }
    else if(n==0)
    {
        printf("XXX  Remote server has closed! recv return n=%d\n",int(n));
        close(clientfd);
    }
    else
    {
        printf("     Failed recved! errno=%d,%s\n",errno, strerror(errno));
        close(clientfd);
    }
    return n;
}


int main(int argc, char** argv)
{
    if(argc < 5)
    {
        printf("Uasge:\n %s <server IP > <port> <timeout msec> <buffer string to send>\n",argv[0]);
        return -1;
    }
    int port = atoi(argv[2]);
	if(port<1)
	{
		fprintf(stderr,"port{%d} error\n",port);
		return 1;
	}
    int timeoutmsec = atoi(argv[3]);
	if(timeoutmsec<1)
	{
		fprintf(stderr,"timeoutmsec{%d} error\n",timeoutmsec);
		return 1;
	}
    
    std::string buf(argv[4]);
	if(buf.size()<1)
	{
		fprintf(stderr,"buf.size{%d} error\n",int(buf.size()));
		return 1;
	}
    
    struct sockaddr_in cli;
    cli.sin_family = AF_INET;
    cli.sin_port = htons(port);
    cli.sin_addr.s_addr = inet_addr(argv[1]);
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0)
    {
        printf("ERROR socket ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }
    
    if(connect(clientfd, (struct sockaddr*)&cli, sizeof(cli)) < 0)
    {
        printf("ERROR connect ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }

    printf("     Connected to %s:%d ,clientfd=%d \n",argv[1],port,clientfd);
        
    int efd = epoll_create(MAXEPOLLSIZE);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = clientfd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, clientfd, &ev) < 0) 
    {
        printf("ERROR epoll_ctl ,errno=%d,%s \n",errno,strerror(errno));
        return 0;
    }
    
    int maxevents = 1;
    struct epoll_event events[MAXEPOLLSIZE];
    while (maxevents>0) 
    {
        int n = epoll_wait(efd, events, maxevents, timeoutmsec);
//		printf("      epoll_wait returned %d \n",n);
        if (n == -1)
        {
            printf("ERROR epoll_wait ,errno=%d,%s \n",errno,strerror(errno));
            return 0;
        }
        if (n == 0)
        {
            send_message(clientfd,buf);
            recv_message(clientfd);
            continue;
        }
     
        for (int i = 0; i < n; ++i)
        {
            int rt = recv_message(clientfd);
            if (rt < 1 && errno != 11)
            {
                epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd,&ev);
                maxevents--;
            }
        }
    }
    
    close(clientfd);
    return 0;
}

