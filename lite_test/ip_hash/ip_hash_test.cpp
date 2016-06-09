
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
using namespace std;

// 输入ip是网络序.
unsigned IPHash(unsigned ip)
{
    unsigned hash = 89;
    unsigned mask[3] ={0xFF000000,0x00FF0000,0x0000FF00};
    unsigned t =0;
    for(unsigned i = 0; i < 3; i++) {
      t =ip&mask[i];
      t =t>>(8*(3-i));
      hash = ( hash * 113 + t ) % 6271; //hash运算
    }
    return hash;
}

// 返回网络序
unsigned IPString2Int(const char * ip)
{
    struct in_addr s;      // IPv4地址结构体
    inet_pton(AF_INET, ip, (void *)&s);
    unsigned ipInt =unsigned(s.s_addr);
    return ipInt;
}

int main() 
{
    std::vector < std::string > _ip;
    int gen_ip_cnt =20;
    for(int i=0;i<gen_ip_cnt;i++)
    {
        char ip[16] ={0};
        snprintf(ip,sizeof(ip),"192.168.%d.%d",i,i+1);
        _ip.push_back(ip);
    }

    for(int j=0;j<gen_ip_cnt;j++)
    {
        unsigned ipInt =IPString2Int(_ip[j].c_str());
        unsigned hash =IPHash(ipInt);
        printf("Hash of %s(%u) is %u \n",_ip[j].c_str(),ipInt,hash);
    }

    return 0;
}
