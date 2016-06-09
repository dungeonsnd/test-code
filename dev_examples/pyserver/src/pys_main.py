#!/bin/env python
# -*- coding: utf-8 -*-
    
import os
import sys
import pys_tcpserver

# default configurations.
conf_var ={ 'process_count':4,
            'start_server_port':8600,
            'log_file':'./pyserver.log',
            'db_host':'127.0.0.1',
            'db_port':6379,
            'db_username':'root',
            'db_passwd':'pwd',
            'db_database':'test', 
            'db_connection_pool_size':32, 
            'coroutine_pool_size_per_process':100000,
            'tcp_backlog':1024, 
            'tcp_listen_on_ip':'0.0.0.0',
            'cache_conf':{}}
            
            
import logging

            
def ParseConf(conffile):
    modname =conffile.rstrip('.py')
    (modpath,modname) =os.path.split(modname)
    sys.path.append(modpath)
    __import__(modname)
    attrs =dir(sys.modules[modname])
    for attr in attrs:
        if conf_var.has_key(attr):
            conf_var[attr] =getattr(sys.modules[modname], attr)

from multiprocessing import Process
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print 'Using default configure files.'
        print "You can use 'nohup python pys_main.py pyserverconf.py &' to use specified conf file."
    else:
        conffile =sys.argv[1]
        print 'conf file:',conffile
        ParseConf(conffile)
        
    logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s %(levelname)s [p%(process)d t%(thread)d] [%(filename)s:%(lineno)d] %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S',
                    filename=conf_var['log_file'],
                    filemode='a')
                
    logging.info('conf_var=%s',str(conf_var))
    l = [ Process(target = pys_tcpserver.Run, args=(i,conf_var)) for i in xrange(0,conf_var['process_count']) ]
    for i in l:
        i.start()
    for i in l:
        i.join()

