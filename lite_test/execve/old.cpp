
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
    
void StartProgram(const char * filename)
{
    int pid =fork();
    if(pid==-1)
    {
        printf("Failed fork,errno=%d,%s \n",errno,strerror(errno));
        return;
    }
    else if(pid==0)
    {
        char *newargv[] = { NULL};
        char *newenviron[] = { NULL };
               
        int rt =execve(filename, newargv, newenviron);
        if(-1==rt)
        {
            printf("Failed execve,errno=%d,%s \n",errno,strerror(errno));
            return;
        }
    }
    else
    {
       //printf("StartProgram parent , pid=%u \n",unsigned(getpid()) );
    }
}

int main(int argc,char * argv[])
{
    printf("old program... , pid=%u \n",unsigned(getpid()) );
    
    system("./new");

//    signal(SIGCHLD,SIG_IGN);
//    StartProgram("/home/test/new");
    
    usleep(1*1000000);
    printf("old program exiting... , pid=%u \n",unsigned(getpid()) );
    return 0;
}
