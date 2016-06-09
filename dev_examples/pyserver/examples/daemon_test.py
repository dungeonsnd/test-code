

import time
import daemon

import gevent
from gevent.server import StreamServer
from gevent.pool import Pool
from gevent.queue import Queue
from gevent import monkey
monkey.patch_socket()

def handler(sock, address):
    print('New connection from %s:%s' % address)

    sock.close()

    
with daemon.DaemonContext():
    server = StreamServer(('0.0.0.0', 18600), handler, backlog=512, spawn=Pool(5000))
    server.serve_forever()

