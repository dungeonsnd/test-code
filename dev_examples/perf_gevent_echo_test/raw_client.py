
"""
[pro@WSCENTOS64_x64 20:35:27 gevent_test]$time python raw_client.py
time= 1410439094.08
time= 1410439104.24

real    0m10.200s
user    0m1.045s
sys     0m5.536s



"""

import socket
import struct
import time

HEAD_LEN=4

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('192.168.1.70',18600))

print 'time=',time.time()

data ='h!'
data =struct.pack('!i',len(data))+data

total =100000

n =0
while n<total:
    n +=1
    sock.sendall(data)
#    print 'sent,n=',n

    header =sock.recv(HEAD_LEN)
    bodyLen =struct.unpack('!i',header)[0]
    body =sock.recv(bodyLen)
#    print body


print 'time=',time.time()
sock.close()

