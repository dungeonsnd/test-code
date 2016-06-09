#ifndef _HEADER_FILE_PLUGIN_INTERFACE_H_
#define _HEADER_FILE_PLUGIN_INTERFACE_H_

#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct PluginData
{
    struct timeval tv;
    struct in_addr ip_src;
    struct in_addr ip_dst;
    unsigned short th_sport;
    unsigned short th_dport;
    unsigned char  th_flags;
    unsigned int th_seq;
    unsigned int th_ack;
    unsigned short th_win;
    unsigned short th_sum;

    int size_ip;
    int size_tcp;
    int size_payload;        
    char * payload;
};

#endif // _HEADER_FILE_PLUGIN_INTERFACE_H_
