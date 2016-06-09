#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char * argv[])
{
    printf("----start download----\n");

    float percent=0;
    float download_size=0;
    float total_size=200;
    float speed=0;

    while(1)
    {
        printf("Download %1.f%% (%.1fMB/%.1fMB), %.1fMB/s                              ",
            percent,download_size,total_size,speed);
        printf("\r");
        fflush(stdout);

        if(download_size==total_size)
        	break;

        usleep(600*1000);
        percent +=5.0;
        download_size +=10.0;
        speed +=2.83;
    }

    printf("\n");
    return 0;
}



