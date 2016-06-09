#!/bin/env python
# -*- coding: utf-8 -*-
    
from pys_common import *
from pys_simple_db_pool import *
from pys_simple_cache_pool import *
import pys_location
import logging
import time

    
def Entry(req,context):
    rsp =None
    db =pys_dbpool.Get()
    cache0 =pys_cachepool.Get('pyscache0')
    cache1 =pys_cachepool.Get('pyscache1')
    try:
        res ={'x':'0','y':'0'}
        uname =cache0.get('s:'+str(req['uid']))
        
        rt =1
        desc =str(req['uid'])+' login failed!'
        if uname: # online
            rt =pys_location.update_location(db,req['uid'],req['x'],req['y'],res)
            if rt==0:
                val =req['x']+'_'+req['y']+'_'+str(time.time())
                cs =cache1.set('s:'+str(req['uid']),val)
                if not cs:
                    rt =2
                rsp ={'cmd':'_uplocationrsp','rt':rt,'x':res['x'],'y':res['y']}  
        else:
            desc =str(req['uid'])+' has not login yet!'

        if rt!=0:
            rsp ={'cmd':'_uplocationrsp','rt':rt,'x':res['x'],'y':res['y'],'desc':desc}  
    except Exception,e:
        print e
    finally:
        pys_cachepool.Release('pyscache1',cache1)
        pys_cachepool.Release('pyscache0',cache0)
        pys_dbpool.Release(db,True)
    return rsp

