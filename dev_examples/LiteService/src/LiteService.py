#!/bin/python 
# -*- coding:utf-8 -*-

import win32serviceutil
import win32service

# next is for logging
import logging
import os
import inspect
import time


'''
#打包后运行方式
LiteService.exe --startup auto  install
LiteService.exe start
LiteService.exe stop
LiteService.exe remove
LiteService.exe install

#让服务自动启动
python LiteService.py --startup auto install
python LiteService.py install
#启动服务
python LiteService.py start
#重启服务
python LiteService.py restart
#停止服务
python LiteService.py stop

#安装后调试程序
python LiteService.py debug
#删除/卸载服务
python LiteService.py remove
了解 Windows 服务体系结构
http://technet.microsoft.com/zh-cn/library/aa998749(EXCHG.65).aspx
'''

"""
Usage: 'LiteService.py [options] install|update|remove|start [...]|stop|restart [...]|debug [...]'
Options for 'install' and 'update' commands only:
 --username domain\username : The Username the service is to run under
 --password password : The password for the username
 --startup [manual|auto|disabled|delayed] : How the service starts, default = manual
 --interactive : Allow the service to interact with the desktop.
 --perfmonini file: .ini file to use for registering performance monitor data
 --perfmondll file: .dll file to use when querying the service for
   performance data, default = perfmondata.dll
Options for 'start' and 'stop' commands only:
 --wait seconds: Wait for the service to actually start or stop.
                 If you specify --wait with the 'stop' option, the service
                 and all dependent services will be stopped, each waiting
                 the specified period.
"""

    
class LiteService(win32serviceutil.ServiceFramework):
    _svc_name_ = "LiteService"
    _svc_display_name_ = "Lite service of xxx."
    _svc_description_='为xxx产品提供服务，一旦停止该服务，有可能影响xxx.'.decode("utf-8")

    def __init__(self, args): 
        win32serviceutil.ServiceFramework.__init__(self, args) 

        self.isAlive = True      
        
        this_file = inspect.getfile(inspect.currentframe())
        dirpath = os.path.abspath(os.path.dirname(this_file))
        logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s %(levelname)s [p%(process)d t%(thread)d] [%(filename)s:%(lineno)d] %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S',
#                    filename=dirpath+"/LiteService.log",
                    filename="C:"+"/LiteService.log",
                    filemode='a')
        logging.info("file_path=%s",dirpath+"/LiteService.log") 

        
    def SvcDoRun(self): 
        logging.info("SvcDoRun start.... ") 
        while self.isAlive:
            logging.info("SvcDoRun alive.")
            time.sleep(5)
        
        
    def SvcStop(self): 
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING) 
        
        self.isAlive = False
        logging.info("SvcStop shutdown....")
    
if __name__=='__main__': 
    if len(sys.argv) == 1:
        servicemanager.PrepareToHostSingle(LiteService)
        servicemanager.Initialize('LiteService', os.path.abspath(servicemanager.__file__))
        servicemanager.StartServiceCtrlDispatcher()
    else:
		win32serviceutil.HandleCommandLine(LiteService)
