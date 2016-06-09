# Echo client program
import socket
import time
import struct

ttl = 1000000
processCnt =5;

"""

TCP Server and this client started four processes seperately.  QPS = 1000000*4processes/5.385s=74w

[pro@WSCENTOS64_x64 06:44:45 perf_libevent]$time python client.py

real    0m5.385s
user    0m11.591s
sys     0m6.799s

"""

HOST = '127.0.0.1'
PORT = 18600

def recvall(sock,size):
    torecv =size
    data =''
    while torecv>0:
        d =sock.recv(torecv)
        data =data+d
        torecv = torecv- len(d)
    return data

def f():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    buf ='abcde'
    lenb =len(buf)

    n =ttl
    while(n>0):
        b =struct.pack('!I',lenb)
        s.sendall(b+buf)
        n =n-1
        
    n =ttl
    while(n>0):
        data = recvall(s,lenb+4)
    #    print data[4:]
        n =n-1

    s.close()

from multiprocessing import Process
ps =[]
for i in range(processCnt):
    p = Process(target=f, args=())
    p.start()
    ps.append(p)
for p in ps:
    p.join()

