
import gevent
from gevent.event import Event

'''
Illustrates the use of events
'''

evt = Event()

def setter():
    '''After 2 seconds, wake all threads waiting on the value of evt'''
    print('setter: Hey wait for me, I have to do something')
    gevent.sleep(2)
    print("setter: Ok, I'm done")
    evt.set()

def waiter():
    '''After 2 seconds the get call will unblock'''
    print("waiter: I'll wait for you")
    evt.wait()  # blocking
    evt.clear()
    print("waiter: wait return")
    gevent.sleep(2)
    
def waiter2():
    '''After 4 seconds the get call will unblock'''
    gevent.sleep(4)
    print("waiter2: I'll wait for you")
    evt.wait()  # blocking
    evt.clear()
    print("waiter2: wait return")

def main():
    gevent.joinall([
        gevent.spawn(setter),
        gevent.spawn(waiter),
        gevent.spawn(waiter)
    ])

if __name__ == '__main__': main()
