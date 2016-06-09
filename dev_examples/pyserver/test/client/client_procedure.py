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
    
def Run(sock,sessionkey,cmd,showlog):
    
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


def Entry(id,showlog):
    # echo/login/uplocation
    
    sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('127.0.0.1',8600+id%server_port_cnt))

    initsession =GenInitSessionData()
    sessionkey =initsession['sessionkey']
    data =PackInitSession(initsession)
    data =PackPackage(data)
    sock.sendall(data)
#    print 'sent InitSession: len(data)=%d'%(len(data))

    Run(sock,sessionkey,'login',showlog)
    print 'login finished.'
    time.sleep(16)
    Run(sock,sessionkey,'uplocation',showlog)
    print 'uplocation finished.'
    time.sleep(16)
    
    print 'ready to close.'
    sock.close()
    
    
if __name__ == '__main__':
    showlog =1
    
    process_cnt =1
    total_req_cnt =1
    server_port_cnt =1
    
    # 多进程
    l = [ Process(target = Entry, args=(i,showlog)) for i in xrange(0,process_cnt) ]
    # 多线程
    #l = [ Thread(target = Run) for i in xrange(0,8) ]
    for i in l:
        i.start()
    for i in l:
        i.join()

    
