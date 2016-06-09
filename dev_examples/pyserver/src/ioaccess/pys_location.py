#!/bin/env python
# -*- coding: utf-8 -*-
    

def update_location(db,uid,x,y,res): 
    cursor = db.cursor() 
    sql ="insert into location(uid,x,y) values(%d,'%s','%s')"%(uid,x,y)
    cursor.execute(sql)
#    print 'affected rowcount=',cursor.rowcount
    rt =1
    if cursor.rowcount==1:
        res['x'] =x
        res['y'] =y
        rt =0
    cursor.close()     
    return rt

