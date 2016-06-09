#!/bin/python 
# -*- coding:utf-8 -*-

from distutils.core import setup
import py2exe

class Target:
    def __init__(self, **kw):
        self.__dict__.update(kw)
        self.version = "0.1"
        self.company_name = "XXXX Co. Ltd."
        self.copyright = "Copyright @2009 XXXX Co., Ltd. "
        self.name = "Lite service of xxx"


myservice = Target(
    description = 'Lite service of xxxx',
    modules = ['LiteService'],
    cmdline_style='pywin32',
    icon_resources=[(1, "ls.ico")]
)

options = {"py2exe":   
            {   "compressed": 1,   
                "bundle_files": 1,
                "dll_excludes": ["w9xpopen.exe"]
            }   
          }  
          
setup(
    service=[myservice],
    options = options, 
    zipfile = None
)
 
