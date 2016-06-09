
import socket
import time

s =socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)
s.connect(('192.168.1.71',18600))

while True:
	s.sendall('abcde')
	buf =s.recv(5)
	print buf
	time.sleep(0.005)

