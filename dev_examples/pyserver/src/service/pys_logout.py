#!/bin/env python
# -*- coding: utf-8 -*-

import time
from pys_common import *
from pys_simple_db_pool import *
from pys_simple_cache_pool import *
import pys_user

    
def Entry(req,context):
    rsp =None
    cache0 =pys_cachepool.Get('pyscache0')
    cache1 =pys_cachepool.Get('pyscache1')
    try:
        if context['uid']:
            cs =cache0.delete('s:'+str(context['uid']))
            cs =cache1.delete('s:'+str(context['uid']))
    except Exception,e:
        print e
    finally:
        pys_cachepool.Release('pyscache1',cache1)
        pys_cachepool.Release('pyscache0',cache0)
    return rsp
