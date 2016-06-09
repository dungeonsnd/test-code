
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/event.h"
#include <event2/dns.h>
#include <WinSock2.h>
#include <Shlobj.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include "lmq_common_thread.hpp"


struct event_base *base;
int connected =-1;
const char * server_host ="im.test.com";


typedef struct {
	struct timeval tv;
	struct event * ev;
	void * timercb_arg;
} timer_param_t;


void readcb(struct bufferevent *bev, void *ctx);
void writecb(struct bufferevent *bev, void *ctx);
void eventcb(struct bufferevent *bev, short events, void *ptr);


static char * evdns_get_default_hosts_filename(void)
{
#ifdef WIN32
	/* Windows is a little coy about where it puts its configuration
	 * files.  Sure, they're _usually_ in C:\windows\system32, but
	 * there's no reason in principle they couldn't be in
	 * W:\hoboken chicken emergency\
	 */
	char path[MAX_PATH+1];
	static const char hostfile[] = "\\drivers\\etc\\hosts";
	char *path_out;
	size_t len_out;

	if (! SHGetSpecialFolderPathA(NULL, path, CSIDL_SYSTEM, 0))
		return NULL;
	len_out = strlen(path)+strlen(hostfile);
	path_out = (char *)malloc(len_out+1);
	evutil_snprintf(path_out, len_out+1, "%s%s", path, hostfile);
	return path_out;
#else
	return mm_strdup("/etc/hosts");
#endif
}



bufferevent * connect_by_host(struct event_base *base, const char * host,int port)
{
	bufferevent * bev =NULL;
	do 
	{
		struct evdns_base *dns_base = evdns_base_new(base, 1);
		char * fname = evdns_get_default_hosts_filename();
		if (0!=evdns_base_load_hosts(dns_base, fname))
		{
			printf("evdns_base_load_hosts failed! \n");
		}		
		if (fname)
			free(fname);

		bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
		if (NULL==bev)
		{
			printf("bufferevent_socket_new failed! \n");
			break;
		}
		printf("++++ bufferevent_socket_new %p \n", bev);

		bufferevent_setcb(bev, readcb, writecb, eventcb, NULL);
		bufferevent_enable(bev, EV_READ|EV_WRITE);

		connected =0;
		int rt =bufferevent_socket_connect_hostname(bev, NULL, AF_INET, host, port);
	} while (0);
	return bev;
}

bufferevent * connect_by_ipport(const char * ip_port)
{
	bufferevent * bev =NULL;
	do 
	{
		struct sockaddr saddr;
		int saddr_len =sizeof(saddr);
		if(0!=evutil_parse_sockaddr_port(ip_port, &saddr, &saddr_len))
		{
			printf("evutil_parse_sockaddr_port failed! \n");
			break;
		}

		bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
		int rt =bufferevent_socket_connect(bev,(struct sockaddr *)&saddr, sizeof(saddr));
		if ( rt!= 0) {
				bufferevent_free(bev);
				break;
		}
		bufferevent_setcb(bev, readcb, writecb, eventcb, NULL);
		bufferevent_enable(bev, EV_READ|EV_WRITE);

	} while (0);
	return bev;
}

int connect_by_host_using_proxy(const char * host,int port,
	const char * pryoxy_ip,int pryoxy_port,
	const char * pryoxy_user,const char * pryoxy_pwd)
{
	int ret =-1;
	do 
	{
		connect_by_host(base, server_host, 443);

		ret =0;
	} while (0);
	return ret;
}


void readcb(struct bufferevent *bev, void *ctx)
{
	char tmp[128];
	size_t n = bufferevent_read(bev, tmp, sizeof(tmp));
	printf("readcb, recved %d bytes: \n", int(n));
	if (n>0)
	{
		for (size_t i=0;i<n;i++)
		{
			printf("%02x ", tmp[i]);
		}
	}
	printf("\n\n");
}

void writecb(struct bufferevent *bev, void *ctx)
{
	printf("writecb, fd=%d \n\n", bufferevent_getfd(bev));
}

void eventcb(struct bufferevent *bev, short events, void *ptr)
{
	int err = EVUTIL_SOCKET_ERROR();
	evutil_socket_t fd =bufferevent_getfd(bev);
	printf("eventcb, fd=%d \n", fd);

    if (events & BEV_EVENT_CONNECTED) {
         /* We're connected to 127.0.0.1:8080.   Ordinarily we'd do
            something here, like start reading or writing. */
		printf("eventcb, BEV_EVENT_CONNECTED \n");
		connected =1;
    } else if (events & BEV_EVENT_ERROR) {
		/* An error occured while connecting. */
		printf("eventcb, BEV_EVENT_ERROR , %d,%s\n", err,evutil_socket_error_to_string(err));
		
		int errdns = bufferevent_socket_get_dns_error(bev);
		if (errdns)
			printf("eventcb, DNS error: %d:%s\n", errdns, evutil_gai_strerror(errdns));
		connected =-1;
	} else{
		/* An error occured while connecting. */
		printf("eventcb, Other ERROR , %d,%s\n", err,evutil_socket_error_to_string(err));
		connected =-1;
	}

	if (connected==-1)
	{
		printf("---- eventcb, bufferevent_free %p \n", bev);
		bufferevent_free(bev);
	}
}


static void timercb_cli(evutil_socket_t ss, short events, void * ctx) 
{
	printf("timercb_cli, connected=%d \n", connected);
	do{
		timer_param_t * param = (timer_param_t *) ctx;
		if(NULL==param)
		{
			break;
		}
		int rt =event_add(param->ev, &param->tv);
		if (0!=rt)
		{
			printf("event_add return %d \n", rt);
		}
		
		if (connected==-1)
		{
			bufferevent *bev =connect_by_host(base, server_host, 443);
			//bufferevent *bev =connect_by_ipport("192.168.28.133:443");
			printf("connect_by_host return %p \n", bev);
			param->timercb_arg =bev;
		}
		else if (connected==0)
		{
		}
		else
		{
			char tmp[9];
			memset(tmp,0,sizeof(tmp));
			bufferevent *bev =(bufferevent *)(param->timercb_arg);
			if (bev)
			{
				bufferevent_write((bufferevent *)(param->timercb_arg), tmp, sizeof(tmp));
			}
			else
			{
				printf("timercb_cli, bev in args is NULL.\n");
			}
		}
		
	}while(0);
	printf("\n");
}

int TcpClient(int & exit, void * arg)
{
	bufferevent *bev =NULL;

	base = event_base_new();

	printf("event_base_dispatch \n");

	{
		timer_param_t * param = new(timer_param_t);
		param->ev = evtimer_new(base, timercb_cli, param);
		param->tv.tv_sec = 0;
		param->tv.tv_usec = 2*1000*1000;
		param->timercb_arg =bev;
		evtimer_add(param->ev, &param->tv);
	}

	int rt =event_base_dispatch(base);
	printf("event_base_dispatch return %d \n", rt);
	return 0;
}

int main(int argc, char **argv)
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

//	int lastActiveTime =(int)time(NULL);
//	LmqThread * thread_cli =new LmqThread(TcpClient, NULL);
	int exit =0;
	TcpClient(exit, NULL);

    return 0;
}
