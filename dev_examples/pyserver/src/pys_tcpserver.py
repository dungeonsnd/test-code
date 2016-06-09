#!/bin/env python
# -*- coding: utf-8 -*-
    

import gevent
from gevent.server import StreamServer
from gevent.pool import Pool
#from gevent.queue import Queue
from gevent import monkey
monkey.patch_socket()

import logging
import os
import sys
sys.path.append('./util')
sys.path.append('./ioaccess')
sys.path.append('./service')

from pys_common import *
from pys_package import *
from pys_module import *
from pys_simple_db_pool import *
from pys_simple_cache_pool import *

def InitSession(task):
    if task.has_key('sock') and task.has_key('req'):
        req =UnpackInitSession(task['req'])
        if not req:
            return None
        if req.has_key('sessionkey'):
            return req['sessionkey']
        else:
            print 'error of req, has no key sessionkey'
            return ''
    else :
        print 'error of task:,',task
        return ''
        
def DoTask(task,context):
    if task.has_key('sock') and task.has_key('req'):
        if task['req']:
            req =DeserializeFilter(task['req'],context['sessionkey'])
            if not req:
                return False
            rsp =CallModule(req['cmd'],req,context)
        else:
            rsp =CallModule('logout',None,context)
        if rsp:
            rsp =SerializeFilter(rsp,context['sessionkey'])
            task['sock'].sendall(PackPackage(rsp))
#                print 'sent,size=%d,%s'%(len(rsp),rsp)
        else : # no rsp to send.
            pass
    else :
        print 'error of task:,',task
    return True

def handler(sock, address):
#    print('New connection from %s:%s' % address)

#    taskQueue =Queue()
#    taskWorker =gevent.spawn(TaskWorker,taskQueue,sock.fileno()) 

    context ={'uid':None,'sessionkey':''}
    while True:
        header = recvall(sock,PYS_HEAD_LEN)
        if not header:
            break
        bodyLen =GetBodyLenByHeader(header)
        if bodyLen > PYS_MAX_BODY_LEN :
            print 'bodyLen=',bodyLen
            break;
        elif bodyLen > 0 :
            req = recvall(sock,bodyLen)
#            print 'recved:size=%d,%s'%(len(req),req)
            if not header:  # error.
                break
            
            task ={'sock':sock,'req':req}
            if context['sessionkey']:
                if not DoTask(task,context):   # 接收和处理在同一协程中
                    break
            else :
                context['sessionkey'] =InitSession(task)
                if not context['sessionkey']:
                    break                
#            taskQueue.put(task)# 接收和处理在不同的协程中
        else:
            print 'error of bodyLen, bodyLen=',bodyLen
            break
#    taskQueue.put({'stop':''})
    task ={'sock':sock,'req':None}
    DoTask(task,context)
    sock.close()
#    print('%s:%s closed' % address)
#    gevent.joinall([taskWorker])

def Run(id,conf_var):    
    pys_dbpool.Init(conf_var['db_connection_pool_size'],
        conf_var['db_host'],
        conf_var['db_port'],
        conf_var['db_username'],
        conf_var['db_passwd'],
        conf_var['db_database'])

    pys_cachepool.Init(conf_var['cache_conf'])
    
    server = StreamServer((conf_var['tcp_listen_on_ip'], conf_var['start_server_port']+id%conf_var['process_count']), handler, backlog=conf_var['tcp_backlog'], spawn=Pool(conf_var['coroutine_pool_size_per_process']))
    
    logging.info('main starting,pid=%d',os.getpid())
    print 'main stared,pid=',os.getpid()
    server.serve_forever()


