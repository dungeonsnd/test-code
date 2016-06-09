
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

int test_multi_processes()
{
    try {
        log4cplus::initialize();
        log4cplus::PropertyConfigurator::doConfigure("./conf_multi_log.properties");
        for(int i=0;i<1;i++)
        {
            pid_t pid =fork();
            if(pid<0)
            {
                printf("Failed fork \n");
                exit(0);
            }
            else if(pid==0)
            {
                for(int i=0;i<1;i++)
                {
                    LOG4CPLUS_INFO(log4cplus::Logger::getInstance("baselogger"),"worker process. BASE" );
                }
                exit(0);
            }
        }
        
        LOG4CPLUS_INFO(log4cplus::Logger::getRoot(),"master process. root" );
    }
    catch(...) {
        printf("Exception...\n");
        LOG4CPLUS_FATAL(log4cplus::Logger::getRoot(), "Exception occured...");
    }
    int st =0;
    wait(&st);
    return 0;
}

void simple_test()
{
    log4cplus::PropertyConfigurator::doConfigure("./conf_multi_log.properties");
    LOG4CPLUS_INFO(log4cplus::Logger::getInstance("baselogger"),"BASE log." );
    LOG4CPLUS_INFO(log4cplus::Logger::getInstance("loginlogger"),"LOGIN log" );
}

int main(int argc,char * argv[])
{
    simple_test();
//    test_multi_processes();
    return 0;
}

