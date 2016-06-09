#!/bin/env python
# -*- coding: utf-8 -*-

import sys
from pys_define import *


def ImportModule(modname):
    if sys.modules.has_key(modname):
        pass
# 用下面的方式动态导入模块，该模块里面静态导入aaa模块(即import aaa)，多次动态执行后aaa会变成None. 为什么？       
#        del sys.modules[modname]
#        __import__(modname)
    else:
        __import__(modname)

    return sys.modules[modname]

def CallModule(cmd,req,context):
    return ImportModule(PYS_SERVICE_MOD_PRE+cmd).Entry(req,context)
    
