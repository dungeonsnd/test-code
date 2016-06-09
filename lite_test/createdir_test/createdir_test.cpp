
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// /temp/a/b/ --> create /temp/a/b
// /tmp/a/b --> create /temp/a
// /temp/a  --> don't create
int CreateDirs(const char * dir,const unsigned len) 
{
	if(len<2)
		return 0;
	char dirname[2048];
	strncpy(dirname,dir,sizeof(dirname)); 
	for(unsigned i=1;i<len;i++) 
	{ 
		if(dirname[i]=='/') 
		{ 
			dirname[i] =0; 
			if(access(dirname,R_OK)!=0   ) 
			{ 
				if(mkdir(dirname,0755)==-1)
					return   -1;
			} 
			dirname[i]   =   '/'; 
		} 
	} 
	return 0; 
}  

int main(int argc,char * argv[])
{
	if(argc<2)
	{
		printf("Usage: %s <path> \n",argv[0]);
		return 0;
	}
	
	CreateDirs(argv[1],strlen(argv[1]));
	return 0;
}

