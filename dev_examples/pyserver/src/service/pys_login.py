#!/bin/env python
# -*- coding: utf-8 -*-

import time
from pys_common import *
from pys_simple_db_pool import *
from pys_simple_cache_pool import *
import pys_user

    
def Entry(req,context):
#    return {'cmd':'_loginrsp','rt':'1','desc':req['name']+' login succ.'}  # 测试不查数据库，直接返回报文.
    rsp =None
    db =pys_dbpool.Get()
    cache =pys_cachepool.Get('pyscache0')
    try:
        res ={'uid':0}
        rt =pys_user.select_user(db,req['name'],req['pwd'],res)
        if rt==0:
            cs =cache.set('s:'+str(res['uid']),req['name'])            
            if not cs:
                print cs
                rt =1
            else: # login ok.
                context['uid'] =res['uid']
                rsp ={'cmd':'_loginrsp','rt':rt,'uid':res['uid']}  
            
        if rt!=0:
            rsp ={'cmd':'_loginrsp','rt':rt,'desc':req['name']+' login failed.'}
    except Exception,e:
        print e
    finally:
        pys_cachepool.Release('pyscache0',cache)
        pys_dbpool.Release(db,True)
    return rsp
