#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>

#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

void handler(int sig);
void handler_child(int sig);


int main(int argc, char *argv[])
{
    if (signal(SIGINT, handler) < 0)
        ERR_EXIT("signal error");
    if (signal(SIGCHLD, SIG_IGN) < 0)
        ERR_EXIT("signal error");

	pid_t pid =fork();
	if(pid<0)
	{
		perror("fork\n");
		return 0;
	}
	else if (pid==0)
	{
 		if (signal(SIGINT, handler_child) < 0)
			ERR_EXIT("sigaction error"); 
		for (; ;)
			pause();
	}
	else
	{
		for (; ;)
			pause();
	}	

    return 0;

}

void handler(int sig)
{
	switch(sig)
	{
	case SIGINT:	
		printf("rev SIGINT=%d,pid=%u \n", sig,unsigned(getpid()));
		break;
	case SIGCHLD:	
		printf("rev SIGCHLD,pid=%u \n",unsigned(getpid()));
		break;
	default:	
		printf("rev sig=%d,pid=%u \n", sig,unsigned(getpid()));
		break;
	}
	
}

void handler_child(int sig)
{
	switch(sig)
	{
	case SIGINT:	
		printf("rev SIGINT=%d,pid=%u \n", sig,unsigned(getpid()));
		break;
	case SIGCHLD:	
		printf("rev SIGCHLD,pid=%u \n",unsigned(getpid()));
		break;
	default:	
		printf("rev sig=%d,pid=%u \n", sig,unsigned(getpid()));
		break;
	}
	
}
