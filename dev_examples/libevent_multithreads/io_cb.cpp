
#include "io_cb.hpp"

extern int threads_cnt;

void readcbNotify(struct bufferevent *bev, void *arg)
{
//    printf("called readcbNotify!\n");
    Ctx * ctx =(Ctx *)arg;    
    T_Queue * queueOutput = ctx->queueOutput;
    T_Sessions * sessions =ctx->sessions;

    struct evbuffer *input= bufferevent_get_input(bev);
    std::string data(4096,'\0');
    evbuffer_remove(input, &data[0], data.size());
    
    try
    {
        while(1)
        {
            QueueEle ele =queueOutput->TryGet();
            int fd =ele.fd;
            std::string & data =ele.data;
            
            T_Sessions::iterator it =sessions->find(fd);
            if(sessions->end()!=it)
            {
                T_spSessionInfo si =it->second;
                struct evbuffer *output =bufferevent_get_output(si->bev);
                evbuffer_add(output, data.c_str(), data.size());
            }
            else
            {
                printf("fd not found in sessions, sessions.size=%d \n",int(sessions->size()));
            }
        };
    }catch (cf::QueueEmptyException & e) // empty now.
    {
    }
}

void readcb(struct bufferevent *bev, void *arg)
{
//    printf("called readcb!\n");
    
    Ctx * ctx =(Ctx *)arg;    
    std::vector < T_spQueue > * queuesInput = ctx->queuesInput;    
    T_Sessions * sessions =ctx->sessions;
    
    struct evbuffer *input= bufferevent_get_input(bev);
//    struct evbuffer *output =bufferevent_get_output(bev);
    
    evutil_socket_t fd =bufferevent_getfd (bev);
    int & recvingHeader =(*sessions)[fd]->recvingHeader;
				
    size_t input_len = evbuffer_get_length(input);  
    while(input_len>0)
    {
        if(recvingHeader)
        {
            if(input_len>=4)
            {
                std::string data(4,'\0');
                evbuffer_remove(input, &data[0], data.size());
                recvingHeader =0;
                unsigned * u =(unsigned *)(&data[0]);
                unsigned size =*u;
                size =ntohl(size);
//				printf("size=%u \n",size);
				if(size>1024000)
				{
					return;
				}
                (*sessions)[fd]->bodySize =size;	    
				
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
            int size =(*sessions)[fd]->bodySize;
//			printf("input_len=%d , size=%u \n",int(input_len),size);
            if(int(input_len)>=size)
            {
                std::string data(size,'\0');
                evbuffer_remove(input, &data[0], data.size());
                
                QueueEle qe;
                qe.fd =fd;
                qe.data =data;
                
                
                (*queuesInput)[fd%threads_cnt]->Put(qe);
            
//                printf("put data to thread queue %d , fd=%d ,  \n", fd%threads_cnt,fd);

//                evbuffer_add(output, bd.c_str(), bd.size());
				
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
void errorcb(struct bufferevent *bev, short error, void *arg)
{
	evutil_socket_t fd =bufferevent_getfd (bev);
    Ctx * ctx =(Ctx *)arg;  
    T_Sessions * sessions =(T_Sessions *)ctx->sessions;
    
    if (error & BEV_EVENT_EOF)
        printf("connection closed, fd=%d \n" ,fd );
    else if (error & BEV_EVENT_ERROR)
    {
		int err =EVUTIL_SOCKET_ERROR();
        printf("some other error(%d),%s , fd=%d\n" ,err,evutil_socket_error_to_string(err) ,fd);
    }
    else if (error & BEV_EVENT_TIMEOUT)
        printf("Timed out\n");

    sessions->erase(fd);
}

void acceptcb(evutil_socket_t listener, short event, void *arg)
{
    Ctx * ctx =(Ctx *)arg;    
    struct event_base *base = ctx->base;   
    T_Sessions * sessions =ctx->sessions;
    struct sockaddr_in in_addr;
    socklen_t slen = sizeof(in_addr);
    int fd = accept(listener, (struct sockaddr*)&in_addr, &slen);
    if (fd < 0)
    {
        perror("accept");
    }
    else if (fd > FD_SETSIZE)
    {
        close(fd);
    }
    else
    {
        struct bufferevent *bev;
        evutil_make_socket_nonblocking(fd);
        
        //使用bufferevent_socket_new创建一个struct bufferevent *bev，关联该sockfd，托管给event_base
        ////BEV_OPT_CLOSE_ON_FREE表示释放bufferevent时关闭底层传输端口。这将关闭底层套接字，释放底层bufferevent等。
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        
        T_spSessionInfo si(new SessionInfo(bev));
        sessions->insert(std::make_pair(fd,si));
    
        //设置读写对应的回调函数
        bufferevent_setcb(bev, readcb, writecb, errorcb, arg);
//      bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
        //启用读写事件,其实是调用了event_add将相应读写事件加入事件监听队列poll。正如文档所说，如果相应事件不置为true，bufferevent是不会读写数据的
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}


void InsertNotifyFds(struct event_base * base,
    const std::vector < T_spPipe > & pipes,
    Ctx * ctx)
{
    for(std::vector < T_spPipe >::const_iterator it =pipes.begin();
        it!=pipes.end();it++)
    {
        struct bufferevent * bev = bufferevent_socket_new(base, (*it)->fd_r, BEV_OPT_CLOSE_ON_FREE);        
        bufferevent_setcb(bev, readcbNotify, NULL, errorcb, ctx);
        bufferevent_enable(bev, EV_READ);
    }
}

