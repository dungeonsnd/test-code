#!/bin/sh


gcc print.c -fPIC -shared -o libprint.so
gcc main.c -c -L./ -lprint -o main.o
gcc main.o -L./ -lprint -Wl,-rpath=../lib:../tlib -o main

