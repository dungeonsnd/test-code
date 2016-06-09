#!/bin/sh

cd plugin
./build.sh
cd ../

gcc -g -Wall custcap.c -o custcap -lpcap -I./plugin/
mv custcap ../bin/


