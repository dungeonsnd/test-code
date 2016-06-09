#ifndef _HEADER_FILE_QUEUE_ELEMENT_HPP_
#define _HEADER_FILE_QUEUE_ELEMENT_HPP_

#include "include_cpp.hpp"
#include "cppfoundation/cf_concurrency_queue.hpp"


struct QueueEle
{
    int fd;
    std::string data;
};

typedef cf::TYPEQueue < QueueEle > ::Queue  T_Queue;
typedef std::tr1::shared_ptr < T_Queue > T_spQueue;

#endif // _HEADER_FILE_QUEUE_ELEMENT_HPP_
