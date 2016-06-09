
import gevent
import pymysql
#import MySQLdb as pymysql  # MySQLdb 是对 mysql-c库进行了包装，所以不支持 monkey.
import time

from gevent import monkey
monkey.patch_socket() 

    
sql1="update t set name='zs-py' where id=4096;"
sql2="select * from t where id=4096;"

db1 =pymysql.connect(host = '192.168.17.153', passwd = 'tm', user = 'root', db= 'test')
db2 =pymysql.connect(host = '192.168.17.153', passwd = 'tm', user = 'root', db= 'test')


def dbQuery1(sql,db): 
    print '---- dbQuery1 start ----'
    cursor = db.cursor() 
    data = cursor.execute(sql) 
    cursor.close() 
    db.commit()
    for e in cursor:
        print e 
    print '---- dbQuery1 over ----'

def dbQuery2(sql,db): 
    print '---- dbQuery2 start ----'
    cursor = db.cursor() 
    data = cursor.execute(sql) 
    cursor.close() 
    db.commit()
    for e in cursor:
        print e
    print '---- dbQuery2 over ----'
    
def normalTask(): 
    n =0
    while n<4:
        print '---- normal task,%d ----'%(n)
        n +=1
        gevent.sleep(1)
        
print time.time()
jobs = [gevent.spawn(normalTask), gevent.spawn(dbQuery1, sql1, db1), gevent.spawn(dbQuery2, sql2, db2)] 
gevent.joinall(jobs, timeout=6) 
print 'time now: ',time.time()

db1.close() 
db2.close() 

