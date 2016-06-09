#!/bin/env python
# -*- coding: utf-8 -*-

import pymysql
import exceptions

import gevent
from gevent import monkey
monkey.patch_socket() 
from gevent.queue import Queue
import time


# TODO: 自动重连机制需要完善，Final中只释放空闲的连接.
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
        while self._dbFree.empty():
            print 'empty'            
            gevent.sleep(0.1)
        conn =self._dbFree.get()
        return conn
    def Release(self,conn,commit):
        if not self._inited:
            return
        if commit:
            conn.commit()
        else:
            conn.rollback()
        print 'put'
        self._dbFree.put(conn)
        gevent.sleep(0)        
        
pys_dbpool =SimpleDBPool()


pys_dbpool.Init(1,'192.168.17.153',3306,'root','tm','test')

def Run():
    conn =pys_dbpool.Get()
    print 'Run'
    cursor = conn.cursor() 
    sql ="update users set name='zs' where uid=1024";
    cursor.execute(sql) 
    cursor.close()
    pys_dbpool.Release(conn,True)

gevent.joinall([
    gevent.spawn(Run),
    gevent.spawn(Run)
])





