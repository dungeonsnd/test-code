
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc,char * argv[])
{
    printf("new program... , pid=%u \n",unsigned(getpid()) );
    usleep(8*1000000);
    printf("new program exiting... , pid=%u \n",unsigned(getpid()) );
    return 0;
}
