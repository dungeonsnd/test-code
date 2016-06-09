

class Actor(gevent.Greenlet):

    def __init__(self):
        self.inbox = Queue()
        Greenlet.__init__(self)

    def onRecv(self, message):
        """
        Define in your subclass.
        """
        raise NotImplemented()

    def _run(self):
        self.running = True

        while self.running:
            message = self.inbox.get()
            self.onRecv(message)
            
import gevent
from gevent.queue import Queue
from gevent import Greenlet

class loginActor(Actor):
    def onRecv(self, message):
        print(message)
        chatAct.inbox.put('loginAct')
        gevent.sleep(0)

class chatActor(Actor):
    def onRecv(self, message):
        print(message)
        loginAct.inbox.put('chatAct')
        gevent.sleep(0)

        
import gevent
from gevent.queue import Queue

def Dispatch(line):
    if line=='loginAct':
        return loginAct.inbox.put('login act returned.')
    else if line=='chatActor':
        return chatActor.inbox.put('chat act returned.')
    else
        return None
        
    
    
import gevent.server

def handler(sock, address):
    fp = sock.makefile()
    while True:
        line = fp.readline()
        
        result =Dispatch(line)
        if len(result)>0:
            fp.write(result)
            fp.flush()
        else if len(result)==0:
            continue
        else :
            break
    sock.shutdown(socket.SHUT_WR)
    sock.close()

if __name__ == '__main__':

    queueSize =4
    queueList =[Queue() for i in xrange(queueSize)]
    
    server = gevent.server.StreamServer(('0.0.0.0', 18600), backlog=1024, handler, spawn=Pool(50000)) # do not accept more than 10000 connections
    server.serve_forever()
    
    
