#ifndef _HEADER_FILE_IO_CONTEXT_HPP_
#define _HEADER_FILE_IO_CONTEXT_HPP_

#include "include_libevent.hpp"
#include "include_system.hpp"
#include "include_cpp.hpp"
#include "session_info.hpp"
#include "queue_element.hpp"
#include "pipe.hpp"

struct Ctx
{
    struct event_base *base;
    T_Sessions * sessions;
    std::vector < T_spQueue > * queuesInput;
    T_Queue * queueOutput;
    std::vector < T_spPipe > * pipes;
};

#endif // _HEADER_FILE_IO_CONTEXT_HPP_
