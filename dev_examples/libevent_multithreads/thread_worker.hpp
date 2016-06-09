#ifndef _HEADER_FILE_THREAD_WORKER_HPP_
#define _HEADER_FILE_THREAD_WORKER_HPP_

#include "include_system.hpp"
#include "include_cpp.hpp"

#include "cppfoundation/cf_concurrency_queue.hpp"

#include "io_context.hpp"


struct ThreadArg
{
    Ctx ctx;
    int index;
};

void * WorkerProc(void * arg);


#endif // _HEADER_FILE_THREAD_WORKER_HPP_
