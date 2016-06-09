#!/bin/python

import sys
sys.path.append('../protocol/')
import im_pb2


people = im_pb2.people()  
people.id = 1234
people.name = "zhangsan"
people.friends.append("lisi")  
people.friends.append("wangwu")  
people_str=people.SerializeToString() 

print people_str

people1 = im_pb2.people()  
people1.ParseFromString(people_str)
print people1.id,people.name

