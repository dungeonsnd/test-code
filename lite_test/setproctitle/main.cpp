#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/prctl.h>


int setproramtitle(int argc , char ** argv, char **environ, const char * newtitle,unsigned newtitleLen)
{
    for(int i=1;i<argc;i++)
        argv[i] =strdup(argv[i]);

	memcpy(argv[0],newtitle,newtitleLen);
	return 0;
}

extern char **environ;
int main(int argc , char *argv[])
{
	int i;
	printf("argc:%d\n" , argc);
	for (i = 0; i < argc; ++i)
	{
		printf("argv[%d]:%s\n" , i , argv[i]);
	}
	printf("evriron:%s,%s,%s,%s\n" , environ[0],environ[1],environ[2],environ[3]);
    
	std::string newtitle("main new title 123");
	setproramtitle(argc , argv, environ, newtitle.c_str(),newtitle.size());

	printf("-------------------\n argc:%d\n" , argc);
	for (i = 0; i < argc; ++i)
	{
		printf("argv[%d]:%s\n" , i , argv[i]);
	}
	printf("evriron:%s,%s,%s,%s\n" , environ[0],environ[1],environ[2],environ[3]);
    
	pause();
	return 0;
}
 