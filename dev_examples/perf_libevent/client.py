# Echo client program
import socket
import time
import struct

ttl = 1000000

"""
[test@192.168.1.2 10:09:16 ~/.git_src/perf_libevent]$time python client.py

real    0m7.141s
user    0m5.488s
sys     0m1.602s
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

    
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
buf ='abcde'

n =ttl
while(n>0):
    b =struct.pack('!I',len(buf))
    s.sendall(b+buf)
    n =n-1
    
n =ttl
while(n>0):
    data = recvall(s,len(buf)+4)
#    print data[4:]
    n =n-1

s.close()

