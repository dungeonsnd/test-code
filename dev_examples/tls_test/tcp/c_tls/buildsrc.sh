#!/bin/bash

g++ -I/usr/local/Cellar/openssl/1.0.2d_1/include/ -L /usr/local/Cellar/openssl/1.0.2d_1/lib/ -o client client.cpp  -lssl  -lcrypto

g++ -I/usr/local/Cellar/openssl/1.0.2d_1/include/ -L /usr/local/Cellar/openssl/1.0.2d_1/lib/ -o server server.cpp  -lssl  -lcrypto
