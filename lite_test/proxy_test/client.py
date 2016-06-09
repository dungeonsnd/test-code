# -*- coding: utf-8 -*-

import socket
import base64

'''
Tencent QQ login when use proxy. From wireshark.

CONNECT 111.161.52.156:80 HTTP/1.1
Host: 111.161.52.156:80
Proxy-Authorization: Basic eGllc2hhb2p1bjpmb2N1czEyMzQ=
Accept: */*
Content-Type: text/html
Proxy-Connection: Keep-Alive
Content-length: 0

'''

#实际主机
HOST = '115.29.170.61'
#非80或443端口时，代理会报502. 
#Received: 'HTTP/1.1 502 Proxy Error ( The specified Secure Sockets Layer (SSL) port is not allowed. Forefront TMG is not configured to allow SSL requests from this port. Most Web browsers use port 443 for SSL requests.  )
PORT = '443'


phost ='192.168.17.190'
pport =8080
puser ='test'
ppasswd ='test'


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((phost, pport))

sendbuf ='CONNECT '+HOST+':'+PORT+' HTTP/1.1\r\n'

#sendbuf =sendbuf+'Host: '+HOST+':'+PORT+'\r\n'    # ---- no need. 注释掉的这些http选项可加可不加.

sendbuf =sendbuf+'Proxy-Authorization: Basic '+base64.b64encode(puser+':'+ppasswd)+'\r\n'

#sendbuf =sendbuf+'Accept: */*'+'\r\n'    # ---- no need.
#sendbuf =sendbuf+'Content-Type: text/html'+'\r\n'    # ---- no need.
#sendbuf =sendbuf+'Proxy-Connection: Keep-Alive'+'\r\n'    # ---- no need.
#sendbuf =sendbuf+'Content-length: 0'+'\r\n'    # ---- no need.

sendbuf =sendbuf+'\r\n'

s.sendall(sendbuf)
print 'sendbuf:\n',sendbuf

data = s.recv(102400) # need to  recv until '\r\n\r\n'. Just for convenient here.
print 'Received:', repr(data)    

print '----------------------------'
for i in range(5):
    s.sendall('Hello!!')
    data = s.recv(1024)
    print repr(data)    

import time
time.sleep(3)
s.close()

