#!/bin/env python
    
from gevent.server import StreamServer
from gevent.pool import Pool
from gevent.queue import Queue
from gevent import monkey
monkey.patch_socket()
import struct
import time


def recvall(sock,totalLen):
    data =''
    left =totalLen
    while left>0:
        d =sock.recv(left)
        if not d:
            data =None
            break
        data +=d
        left =totalLen-len(data)
    return data

    
def handler(sock, address):
    print('New connection from %s:%s' % address)

    
    HEAD_LEN=4
    MAX_BODY_LEN=10240
    while True:
        header = recvall(sock,HEAD_LEN)
        if not header:
            break
        bodyLen =struct.unpack('!i',header)[0]
        if bodyLen > MAX_BODY_LEN :
            print 'bodyLen=',bodyLen
            break;
        elif bodyLen > 0 :
            body = recvall(sock,bodyLen)
#            print 'recved:',body
            if not header:
                break
            rsp =body
            rsp =struct.pack('!i',len(rsp))+body
            sock.sendall(rsp)
#            print 'sent'
    sock.close()
    
if __name__ == '__main__':
    server = StreamServer(('0.0.0.0', 18600), handler, backlog=512, spawn=Pool(5000))
    server.serve_forever()
    
    
