
import time
t1 = time.time()
from gevent import monkey
monkey.patch_socket()
import redis
r_server = redis.Redis("192.168.17.153")
a=r_server.set("name", "DeGizmo")
for i in xrange(10000):
    b=r_server.get("name")

print time.time()-t1
