#ifndef _HEADER_FILE_SESSION_INFO_HPP_
#define _HEADER_FILE_SESSION_INFO_HPP_

#include "include_libevent.hpp"
#include "include_system.hpp"
#include "include_cpp.hpp"


class SessionInfo
{
public:
    int recvingHeader;
    int bodySize;
    struct bufferevent * bev;
    
    SessionInfo(struct bufferevent * b):
        recvingHeader(1),bodySize(0),bev(b)
    {
    }
    ~SessionInfo()
    {
        if(bev)
            bufferevent_free(bev);
    }
};
typedef std::tr1::shared_ptr < SessionInfo > T_spSessionInfo;
typedef std::tr1::unordered_map < int , T_spSessionInfo > T_Sessions;


#endif // _HEADER_FILE_SESSION_INFO_HPP_
