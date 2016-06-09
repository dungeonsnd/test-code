# Echo client program
import socket
import time
import struct

ttl = 40000

"""
[pro@192.168.1.78 15:21:12 ~/go_echo_test]$time python client.py

real    0m13.610s
user    0m11.360s
sys     0m15.391s
[pro@192.168.1.78 15:21:32 ~/go_echo_test]$
"""

HOST = '127.0.0.1'
PORT = 18080

def recvall(sock,size):
    torecv =size
    data =''
    while torecv>0:
        d =sock.recv(torecv)
        data =data+d
        torecv = torecv- len(d)
    return data

def Run():    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    buf ='abcde'

    n =ttl
    while(n>0):
        b =struct.pack('!I',len(buf))
        s.sendall(b+buf)
        
        data = recvall(s,len(buf)+4)
    #    print data[4:]
        n =n-1

    s.close()


from multiprocessing import Process
pList=[]
for i in range(8):
    p = Process(target=Run)
    pList.append(p)

for p in pList:
    p.start()
for p in pList:
    p.join()

