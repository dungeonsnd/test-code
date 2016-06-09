#!/bin/env python
# -*- coding: utf-8 -*-

import pymysql
import exceptions

from gevent import monkey
monkey.patch_all() 
from gevent.queue import Queue
import gevent

# TODO : auto-reconnect to mysql when disconnected.
class SimpleDBPool():
    DBPOOL_SIZE=0
    HOST=''
    PORT=0
    USERNAME=''
    PASSWD=''
    DATABASE=''
    def __init__(self):
        self._dbFree =Queue()
        self._inited =False
        
    def Init(self,dbpool_size,host,port,username,passwd,database):
        self.DBPOOL_SIZE=dbpool_size
        self.HOST=host
        self.PORT=port
        self.USERNAME=username
        self.PASSWD=passwd
        self.DATABASE=database
        
        for conn in [pymysql.connect(host=self.HOST,port=self.PORT,user=self.USERNAME,passwd=self.PASSWD,db=self.DATABASE) for i in xrange(0,self.DBPOOL_SIZE)] :
            self._dbFree.put(conn)
        self._inited =True
    def Final(self):
        for conn in self._dbFree :
            conn.close()
        self._inited =False
            
    def Get(self):
        if not self._inited:
            return
#        while self._dbFree.empty():
#            print 'db pool empty! pys_simple_db_pool.py:44'
#            gevent.sleep(0.001)
        conn =self._dbFree.get()
        return conn
    def Release(self,conn,commit):
        if not self._inited:
            return
        if commit:
            conn.commit()
        else:
            conn.rollback()
        self._dbFree.put(conn)
        gevent.sleep(0)        
        
pys_dbpool =SimpleDBPool()

