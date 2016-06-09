#ifndef _HEADER_FILE_LOG4CPLUS_WRAPPER_
#define _HEADER_FILE_LOG4CPLUS_WRAPPER_

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include "log4cplus/loggingmacros.h"

#define LEVEL_TRACE 100
#define LEVEL_DEBUG 200
#define LEVEL_INFO  300
#define LEVEL_WARN  400
#define LEVEL_ERROR 500
#define LEVEL_FATAL 600

#define log_init(property) \
{ \
    log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(false); \
    log4cplus::PropertyConfigurator::doConfigure(property); \
}

#define log_set(loggername,level)   \
{  \
    int ll;  \
    switch(level)   \
    {   \
    case LEVEL_TRACE:   \
        ll =log4cplus::TRACE_LOG_LEVEL;  \
        break;   \
    case LEVEL_DEBUG:   \
        ll =log4cplus::DEBUG_LOG_LEVEL;  \
        break;   \
    case LEVEL_INFO:   \
        ll =log4cplus::INFO_LOG_LEVEL;  \
        break;   \
    case LEVEL_WARN:   \
        ll =log4cplus::WARN_LOG_LEVEL;  \
        break;   \
    case LEVEL_ERROR:   \
        ll =log4cplus::ERROR_LOG_LEVEL;  \
        break;   \
    case LEVEL_FATAL:   \
        ll =log4cplus::FATAL_LOG_LEVEL;  \
        break;   \
    default:   \
        ll =log4cplus::ALL_LOG_LEVEL;  \
        break;   \
    }  \
    log4cplus::Logger::getInstance(loggername).setLogLevel(ll);   \
}

#define log_t(loggername,...) LOG4CPLUS_TRACE_FMT(log4cplus::Logger::getInstance(loggername), __VA_ARGS__);
#define log_d(loggername,...) LOG4CPLUS_DEBUG_FMT(log4cplus::Logger::getInstance(loggername), __VA_ARGS__);
#define log_i(loggername,...) LOG4CPLUS_INFO_FMT(log4cplus::Logger::getInstance(loggername), __VA_ARGS__);
#define log_w(loggername,...) LOG4CPLUS_WARN_FMT(log4cplus::Logger::getInstance(loggername), __VA_ARGS__);
#define log_e(loggername,...) LOG4CPLUS_ERROR_FMT(log4cplus::Logger::getInstance(loggername), __VA_ARGS__);
#define log_f(loggername,...) LOG4CPLUS_FATAL_FMT(log4cplus::Logger::getInstance(loggername), __VA_ARGS__);


#endif // _HEADER_FILE_LOG4CPLUS_WRAPPER_
