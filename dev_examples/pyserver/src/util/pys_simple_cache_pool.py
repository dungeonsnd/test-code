#!/bin/env python
# -*- coding: utf-8 -*-

import redis
from gevent import monkey
monkey.patch_all() 
from gevent.queue import Queue
import gevent

# TODO: 自动重连机制需要完善，Final中只释放空闲的连接.
class SimpleCachePool():
    def __init__(self):
        self._pool ={}
        self._cacheFree ={}
        self._inited =False

    def Init(self,pool):
        self._pool =pool
        for cache_id in self._pool: 
            self._cacheFree[cache_id] =Queue()
            for conn in [ redis.StrictRedis(host =self._pool[cache_id]['cache_host'],
                          port =self._pool[cache_id]['cache_port'],
                          db =self._pool[cache_id]['cache_database']) 
                            for i in xrange(0,self._pool[cache_id]['cache_connection_pool_size']) ] :
                self._cacheFree[cache_id].put(conn)
        self._inited =True
    def Final(self):
        for q in self._cacheFree :
            for conn in q :
                conn.close()
        self._inited =False
            
    def Get(self,cache_id):
        if not self._inited:
            return
        conn =self._cacheFree[cache_id].get()
        return conn
    def Release(self,cache_id,conn):
        if not self._inited:
            return
        self._cacheFree[cache_id].put(conn)
        gevent.sleep(0)        

pys_cachepool =SimpleCachePool()

