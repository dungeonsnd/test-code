#!/bin/env python
# -*- coding: utf-8 -*-
"""
服务器4进程，开4个端口
客户端8进程，每进程40000次echo请求

real    0m19.643s
user    0m24.450s
sys     0m6.231s

QPS=32w/20s=1.6w
"""

import socket
import time
from threading import Thread
from multiprocessing import Process
import sys
sys.path.append('../../src/util/')
from pys_define import *

from pys_package import *
from pys_common import *
from pys_raw_socket import *

process_cnt =1
total_req_cnt =1

server_port_cnt =1

req_echo ={u'cmd':u'echo',u'data':u'hello,world!'}
req_login ={'cmd':'login','name':u'zhangsan'.encode(u'UTF-8'),u'pwd':u'123456'}
req_uplocation ={u'cmd':u'uplocation',u'uid':1024,'x':'100.101','y':'200.202'}

reqs={u'echo':req_echo,u'login':req_login,u'uplocation':req_uplocation}
    
def Run(id,cmd,showlog):
    sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('127.0.0.1',8600+id%server_port_cnt))

    #print 'time=',time.time()
    
    initsession =GenInitSessionData()
    sessionkey =initsession['sessionkey']
    data =PackInitSession(initsession)
    data =PackPackage(data)
    sock.sendall(data)
#    print 'sent InitSession: len(data)=%d'%(len(data))
    
    req =reqs[cmd]
    data =SerializeFilter(req,sessionkey)
    data =PackPackage(data)

    n =0
    while n<total_req_cnt:
        n +=1
        sock.sendall(data)
        if showlog:
            print 'sent: n=%d,len(data)=%d,req=%s'%(n,len(data),req)

        header =recvall(sock,PYS_HEAD_LEN)
        if len(header) <1:
            print 'peer closed this session when recv header!'
            break
        bodyLen =GetBodyLenByHeader(header)
        recvdata =recvall(sock,bodyLen)
        if len(recvdata) <1:
            print 'peer closed this session when recv body!'
            break
        rsp =DeserializeFilter(recvdata,sessionkey)
        if showlog:
            print 'recved: n=%d,len(data)=%d,rsp=%s'%(n,PYS_HEAD_LEN+len(recvdata),rsp)

    #print 'time=',time.time()
    sock.close()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: %s <echo/login/uplocation> <show send/recv 1/0> [process_cnt] [total_req_cnt] [server_port_cnt] \n'%(sys.argv[0])
    else:
        cmd =sys.argv[1]
        showlog =int(sys.argv[2])
        
        if len(sys.argv) >= 4:
            process_cnt =int(sys.argv[3])
        if len(sys.argv) >= 5:
            total_req_cnt =int(sys.argv[4])
        if len(sys.argv) >= 6:
            server_port_cnt =int(sys.argv[5])
        
        # 多进程
        l = [ Process(target = Run, args=(i,cmd,showlog)) for i in xrange(0,process_cnt) ]
        # 多线程
        #l = [ Thread(target = Run) for i in xrange(0,8) ]
        for i in l:
            i.start()
        for i in l:
            i.join()

    
