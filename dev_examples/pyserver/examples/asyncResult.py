
#  http://xlambda.com/gevent-tutorial/#_8

import time
import gevent
from gevent.event import AsyncResult
a = AsyncResult()

def setter():
    """
    After 3 seconds set the result of a.
    """
    gevent.sleep(2)
    a.set('Hello!')

def waiter1():
    """
    After 3 seconds the get call will unblock after the setter
    puts a value into the AsyncResult.
    """
    s =a.get()
    print 'waiter1,',s

def waiter2():
    """
    After 3 seconds the get call will unblock after the setter
    puts a value into the AsyncResult.
    """
    gevent.sleep(4)
    s =a.get()
    print 'waiter2,',s
    
def otherTask():
    n =0
    while n<6:
        n +=1
        print 'other task.'
        gevent.sleep(1)
    
gevent.joinall([
    gevent.spawn(setter),
    gevent.spawn(waiter1),
    gevent.spawn(waiter2),
    gevent.spawn(otherTask)
])
