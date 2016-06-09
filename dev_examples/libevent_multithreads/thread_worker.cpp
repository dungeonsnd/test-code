
#include "thread_worker.hpp"

void * WorkerProc(void * arg)
{
    ThreadArg * tharg =(ThreadArg *)arg;
    int index =tharg->index;
    Ctx ctx =tharg->ctx;
    std::vector < T_spQueue > * queuesInput =ctx.queuesInput;
    T_Queue * queueOutput =ctx.queueOutput;
    std::vector < T_spPipe > * pipes =ctx.pipes;
    
    bool stop =false;
    while(false==stop)
    {
//        printf("Thread %d ,before get \n",index);
        QueueEle qe =(*queuesInput)[index]->Get();
        
//        printf("Thread %d ,fd =%d \n",index, qe.fd);
        
        unsigned t=qe.data.size();
        std::string bd(t+4,'\0');
        t=htonl(t);
        memcpy(&bd[0],&t,sizeof(unsigned));
        memcpy(&bd[4],qe.data.c_str(), qe.data.size());

        QueueEle qeOutput;
        qeOutput.fd =qe.fd;
        qeOutput.data =bd;

        queueOutput->Put(qeOutput);
        
        write((*pipes)[index]->fd_w,"a",1);
    }
    return NULL;
}

