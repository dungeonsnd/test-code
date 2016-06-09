#!/bin/sh

cd ../protocol/ ; ./build.sh ; cd ../src/

g++ -Wall -g -Wl,-rpath=../protobuf-2.5.0/lib/  main.cpp message.pb.cc -o main -I ../protobuf-2.5.0/include/google/protobuf/ -I../protobuf-2.5.0/include/ -L ../protobuf-2.5.0/lib/ -lprotobuf
