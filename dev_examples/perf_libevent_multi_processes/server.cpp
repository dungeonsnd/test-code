
#include <tr1/unordered_map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

std::tr1::unordered_map < int , char > _fd_recvHeader;
std::tr1::unordered_map < int , unsigned > _fd_sessionInfo;

int processCnt =5;

struct event *listener_event =NULL;
    
void readcb(struct bufferevent *bev, void *ctx)
{
#if 0       
    printf("called readcb!\n");
#endif
    struct evbuffer *input= bufferevent_get_input(bev);
    struct evbuffer *output =bufferevent_get_output(bev);
    
    evutil_socket_t fd =bufferevent_getfd (bev);
    char & recvingHeader =_fd_recvHeader[fd];
				
    size_t input_len = evbuffer_get_length(input);  
    while(input_len>0)
    {
        if(recvingHeader)
        {
            if(input_len>=4)
            {
                std::string data(4,'\0');
                evbuffer_remove(input, &data[0], data.size());
//		printf("raw = %x,%x,%x,%x \n",0xff&data[0],0xff&data[1],0xff&data[2],0xff&data[3]);
                recvingHeader =0;
                unsigned * u =(unsigned *)(&data[0]);
                unsigned size =*u;
                size =ntohl(size);
//				printf("size=%u \n",size);
				if(size>1000)
				{
					exit(0);
				}
                _fd_sessionInfo[fd] =size;	    
				
                input_len = evbuffer_get_length(input);
//				printf("input_len=%d \n",int(input_len));
                if(input_len<size)
                    break;
            }
            else
                break;
        }
        else
        {
            unsigned size =_fd_sessionInfo[fd];
//			printf("input_len=%d , size=%u \n",int(input_len),size);
            if(input_len>=size)
            {
                std::string data(size,'\0');
                evbuffer_remove(input, &data[0], data.size());
            
                unsigned t=data.size();
                std::string bd(t+4,'\0');
                t=htonl(t);
                memcpy(&bd[0],&t,sizeof(unsigned));
                memcpy(&bd[4],data.c_str(), data.size());
	
		evbuffer_add(output, bd.c_str(), bd.size());
				
                recvingHeader =1;
                input_len = evbuffer_get_length(input);
                if(input_len<4)
                    break;
            }
            else
                break;
        }
    }
}

void writecb(struct bufferevent *bev,void *ctx)
{
#if 0        
    printf("called writecb!\n"); 
#endif       
}
void errorcb(struct bufferevent *bev, short error, void *ctx)
{
#if 0
    printf("called errorcb!\n");
#endif
    if (error & BEV_EVENT_EOF)
    {
        /* connection has been closed, do any clean up here */
        printf("connection closed\n");
    }
    else if (error & BEV_EVENT_ERROR)
    {
        /* check errno to see what error occurred */
		int err =EVUTIL_SOCKET_ERROR();
        printf("some other error(%d),%s \n",err,evutil_socket_error_to_string(err));
		
		evutil_socket_t fd =bufferevent_getfd (bev);
		if(ECONNRESET ==err)
		{
			_fd_recvHeader.erase(fd);
			_fd_sessionInfo.erase(fd);
		}
    }
    else if (error & BEV_EVENT_TIMEOUT)
    {
        /* must be a timeout event handle, handle it */
        printf("Timed out\n");
    }
    // BEV_OPT_CLOSE_ON_FREE
//    close(ci->fd);
//    bufferevent_free(ci->bev);
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
    struct event_base *base = (event_base *)arg;
    struct sockaddr_in in_addr;
    socklen_t slen = sizeof(in_addr);
    int fd = accept(listener, (struct sockaddr*)&in_addr, &slen);
    if (fd < 0)
    {
        printf("Unaccept by process %d \n",int(getpid()));
    }
    else if (fd > FD_SETSIZE)
    {
        close(fd);
    }
    else
    {
        event_del(listener_event);
        printf("Accept by process %d ,fd=%d \n",int(getpid()), fd);
    
        struct bufferevent *bev;
        evutil_make_socket_nonblocking(fd);

        _fd_recvHeader[fd] =1;
        
        //使用bufferevent_socket_new创建一个struct bufferevent *bev，关联该sockfd，托管给event_base
        ////BEV_OPT_CLOSE_ON_FREE表示释放bufferevent时关闭底层传输端口。这将关闭底层套接字，释放底层bufferevent等。
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		
        //设置读写对应的回调函数
        bufferevent_setcb(bev, readcb, writecb, errorcb, NULL);
//      bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
        //启用读写事件,其实是调用了event_add将相应读写事件加入事件监听队列poll。正如文档所说，如果相应事件不置为true，bufferevent是不会读写数据的
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        printf("Usage: %s <port> \n",argv[0]);
        return 0;
    }
    
    int port =atoi(argv[1]);
    
	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigemptyset(&sa.sa_mask) == -1 || sigaction(SIGPIPE, &sa, 0) == -1) 
	{
		perror("failed to ignore SIGPIPE; sigaction");
		exit(EXIT_FAILURE);
	}

    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(port);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        return 0;
    }
	if (listen(listener, 16)<0)
    {
        perror("listen");
        return 0;
    }

    std::vector < pid_t > pids;
    for(int i=0;i<processCnt;i++)
    {
        pid_t pid =fork();
        if(0==pid) // child
        {
            base = event_base_new();
            if (!base)
                return 0;
            listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);

            event_add(listener_event, NULL);
            
            event_base_dispatch(base);
            
            exit(0);
        }
        else if (pid>0) // parent
        {
            pids.push_back(pid);
        }
        else
        {
            printf("error of fork,%d \n",errno);
        }
    }

    int status;
    for(int i=0;i<int(pids.size());i++)
    {
        waitpid(pids[i],&status,WUNTRACED | WCONTINUED);
    }
    return 0;
}
