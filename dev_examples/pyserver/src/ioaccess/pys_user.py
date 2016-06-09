#!/bin/env python
# -*- coding: utf-8 -*-
    

def select_user(db,user,passwd,res): 
    cursor = db.cursor() 
    sql ="select uid,passwd from users where name='%s' and passwd='%s' "%(user,passwd)
#    sql ="update users set name='zhangsan' where id=1024"  # 测试协程被阻塞的情况.
    cursor.execute(sql) 
    rt =-1
    if cursor.rowcount >0:
        data =cursor.fetchall()
        if len(data)==1 :
            rt_uid,rt_passwd =data[0]
            if passwd==rt_passwd:
                rt =0
                res['uid'] =rt_uid
        elif len(data)==0 :
            pass
        else:
            # warning.
            pass
    cursor.close()     
    return rt

