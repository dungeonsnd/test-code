#!/bin/sh

EXE=/usr/local/protobuf-2.5.0/bin/protoc

$EXE  -I. --cpp_out=./ msgA.proto
$EXE  -I. --cpp_out=./ msgB.proto


g++ -Wall -g -Wl,-rpath=.:/usr/local/protobuf-2.5.0/lib -I/usr/local/protobuf-2.5.0/include/ -I./proto/ \
-fPIC -shared msgA.pb.cc msgB.pb.cc -o libmsg.so

