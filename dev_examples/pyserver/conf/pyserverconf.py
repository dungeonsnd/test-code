#!/bin/env python
# -*- coding: utf-8 -*-


process_count = 1
start_server_port =8600
log_file ='../log/pyserver.log'
db_host ='192.168.17.153'
db_port =3306
db_username ='root'
db_passwd ='tm'
db_database ='test'
db_connection_pool_size =16
coroutine_pool_size_per_process =100000
tcp_backlog =1024
tcp_listen_on_ip ='0.0.0.0'

cache_conf ={'pyscache0':   {'cache_host':'192.168.17.153',
                            'cache_port':6379,
                            'cache_database':15,
                            'cache_connection_pool_size':4},
             'pyscache1':   {'cache_host':'192.168.17.153',
                            'cache_port':6379,
                            'cache_database':14,
                            'cache_connection_pool_size':4}
            }
