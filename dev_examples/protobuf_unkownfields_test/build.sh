#!/bin/sh


g++ -Wall -g -Wl,-rpath=.:./proto/:/usr/local/protobuf-2.5.0/lib -I/usr/local/protobuf-2.5.0/include/ -I./proto/ -L./proto/ -L/usr/local/protobuf-2.5.0/lib/ -lmsg -lprotobuf \
pro.cpp -o pro

