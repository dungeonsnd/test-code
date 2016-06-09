
#include "io_cb.hpp"
#include "thread_worker.hpp"

int threads_cnt =2;

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
    struct event *listener_event;

    base = event_base_new();
    if (!base)
        return 0;

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

    T_Sessions sessions;

    std::vector < T_spPipe > pipes;
    for(int i=0;i<threads_cnt;i++)
    {
        T_spPipe pipe(new Pipe());
        pipes.push_back(pipe);
    }
    
    T_Queue queueInput;
    
    std::vector < T_spQueue > queuesInput;
    for(int i=0;i<threads_cnt;i++)
    {
        T_spQueue spQueue(new T_Queue());
        queuesInput.push_back(spQueue);
    }
    
    
    T_Queue queueOutput;
    Ctx ctx;
    ctx.base =base;
    ctx.sessions =&sessions;
    ctx.queuesInput =&queuesInput;
    ctx.queueOutput =&queueOutput;
    ctx.pipes =&pipes;

    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, acceptcb, (void*)(&ctx));
    event_add(listener_event, NULL);
 
    int rt =0;
    std::vector < pthread_t > tids(threads_cnt,'\0');    
    std::vector < ThreadArg > thrags;    
    for(int i=0;i<int(tids.size());i++)
    {
        ThreadArg tharg;
        tharg.ctx =ctx;
        tharg.index =i;
        
        thrags.push_back(tharg);
        
        rt =pthread_create(&tids[i],NULL,WorkerProc,&thrags[i]);
        if(0!=rt)
        {
            printf("Failed pthread_create ,rt=%d \n",rt);
        }
    }
    
    InsertNotifyFds(base,pipes,&ctx);
    
    event_base_dispatch(base);
    
    for(int i=0;i<int(tids.size());i++)
    {
        rt =pthread_join(tids[i],NULL);
        if(0!=rt)
        {
            printf("Failed pthread_join ,rt=%d \n",rt);
        }
    }
    
    return 0;
}


