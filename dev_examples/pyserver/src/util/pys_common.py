#!/bin/env python
# -*- coding: utf-8 -*-

from gevent import monkey
monkey.patch_socket() 

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


