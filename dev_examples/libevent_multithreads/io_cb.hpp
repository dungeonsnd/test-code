#ifndef _HEADER_FILE_IO_CB_HPP_
#define _HEADER_FILE_IO_CB_HPP_

#include "io_context.hpp"

void readcbNotify(struct bufferevent *bev, void *arg);
void readcb(struct bufferevent *bev, void *arg);
void writecb(struct bufferevent *bev,void *ctx);
void acceptcb(evutil_socket_t listener, short event, void *arg);
void InsertNotifyFds(struct event_base * base,
    const std::vector < T_spPipe > & pipes,
    Ctx * ctx);

#endif // _HEADER_FILE_IO_CB_HPP_
