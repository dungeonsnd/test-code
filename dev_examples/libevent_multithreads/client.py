# Echo client program
import socket
import time
import struct
import threading

"""
[test@192.168.1.2 09:32:23 ~/.git_src/libevent_multithreads]$time python client.py

real    0m2.013s
user    0m1.940s
sys     0m2.800s
"""

ttl =10000

threads_cnt = 10

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

def thread_proc(nloop):
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
#        print data[4:]
        n =n-1
        
    s.close()

if __name__ == '__main__':
    threads = []
    nloops = range(threads_cnt)
        
    for i in nloops:
        t = threading.Thread(target = thread_proc, args = (i,))
        threads.append(t)

    for i in nloops:        # start threads
        threads[i].start()
        
    for i in nloops:        #wait for all
        threads[i].join()   #threads to finish

