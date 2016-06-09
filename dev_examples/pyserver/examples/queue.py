
import time
import gevent
import gevent.queue
from gevent.queue import *

tasks = Queue()

def worker():
    while True:
        print 'loop'
        try:
            task = tasks.get_nowait()
            print('got task %s' % (task))
        except gevent.queue.Empty as e :
            print e
        gevent.sleep(0)
    print('Quitting worker!')
 

gevent.spawn(worker)
    
tasks.put('task-1')
tasks.put('task-2')
time.sleep(3)
tasks.put('task-3')

