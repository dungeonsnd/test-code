
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

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

void WriteLog()
{
    std::string buf(128,'a');
    LOG4CPLUS_INFO(Logger::getRoot(), buf.c_str() );
}


int test_single_processes()
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./conf_normal.properties"));
    WriteLog();
    return 0;
}

int test_multi_processes()
{
    log4cplus::initialize();
    try {
        PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./conf_normal.properties"));
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
                for(int i=0;i<1000000;i++)
                    WriteLog();
                exit(0);
            }
        }
        
        WriteLog();
    }
    catch(...) {
        tcout << "Exception..." << endl;
        LOG4CPLUS_FATAL(Logger::getRoot(), "Exception occured...");
    }
    
    int st =0;
    wait(&st);
    return 0;
}

// 把log4cplus配置文件File项增加extend字符串，即修改log4cplus保存的日志文件名称。你可以增加进程pid等信息到文件名称中。
// key是你的log4cplus配置文件中 File项的名称,如果key为空,则搜索全部findAppenderString字符串的File项。
int Log4cplus_AddExtenToFilename(PropertyConfigurator & config,
                                    const std::string & extend,
                                    std::string key ="",
                                    std::string findAppenderString ="Appender.File")
{
    int res =-1;
    log4cplus::helpers::Properties & properties =(log4cplus::helpers::Properties &)(config.getProperties());
    if(key.empty())
    {
        std::vector<log4cplus::tstring> names=properties.propertyNames();
        for(std::vector<log4cplus::tstring>::iterator it =names.begin();
            it!=names.end();it++)
            {
                if(string::npos!=it->find(findAppenderString))
                {
                    key =it->substr(0,it->find(findAppenderString)+findAppenderString.size());
                    std::string newname =properties.getProperty(key.c_str())+extend;
                    //printf("%s= %s, newname=%s \n",it->c_str(),properties.getProperty(key.c_str()).c_str(),newname.c_str());
                    properties.setProperty(key.c_str(),newname.c_str());
                    res =0;
                }            
            }
    }
    else
    {
        std::string newname =properties.getProperty(key.c_str())+extend;
        //printf("%s= %s, newname=%s \n",key.c_str(),properties.getProperty(key.c_str()).c_str(),newname.c_str());
        properties.setProperty(key.c_str(),newname.c_str());
    }
    return res;
}

int GenerateExtend(std::string & extend)
{
    // 获取当时时间放入文件名中.
    struct timeval tv;
    if(0!=gettimeofday(&tv, NULL))
    {
        printf("gettimeofday failed! \n");
        return -1;
    }
    struct tm gmt;
    gmtime_r(&tv.tv_sec, &gmt);

    // 获取pid放入文件名中.
    extend.assign(32,'\0');
    snprintf(&extend[0],extend.size(),".%d.%4d%02d%02d_%02d%02d%02d.log",
        getpid(),
        gmt.tm_year+1900,gmt.tm_mon+1,gmt.tm_mday,
        gmt.tm_hour,gmt.tm_min,gmt.tm_sec);
    extend.resize(strlen(extend.c_str()));
    
    return 0;
}

int test_multi_processes_async_and_modify_logfile_name()
{
    log4cplus::initialize();
    try {
        PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./conf_async.properties"));
        for(int i=0;i<5;i++)
        {
            pid_t pid =fork();
            if(pid<0)
            {
                printf("Failed fork \n");
                exit(0);
            }
            else if(pid==0)
            {
                PropertyConfigurator config("./conf_async.properties");
                
                std::string extend;
                int rt =GenerateExtend(extend);
                if(0!=rt)
                {
                    printf("GenerateExtend failed! \n");
                    exit(0);
                }
                rt =Log4cplus_AddExtenToFilename(config,extend);
                if(0!=rt)
                {
                    printf("Log4cplus_AddExtenToFilename failed! \n");
                    exit(0);
                }
                config.configure();
    
                for(int i=0;i<1000000;i++)
                    WriteLog();
                exit(0);
            }
        }
        
        WriteLog();
    }
    catch(...) {
        tcout << "Exception..." << endl;
        LOG4CPLUS_FATAL(Logger::getRoot(), "Exception occured...");
    }
    
    for(int i=0;i<5;i++)
    {
        int st =0;
        wait(&st);
    }
    return 0;
}

int main(int argc,char * argv[])
{
//    return test_single_processes();
//    return test_multi_processes();
    return test_multi_processes_async_and_modify_logfile_name();
}
