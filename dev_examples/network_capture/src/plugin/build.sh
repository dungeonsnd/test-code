#!/bin/sh

gcc -g -Wall -fPIC -shared plugin_myapp_parser.c plugin_myapp_proto.c -o libplugin-myapp.so

mv libplugin-myapp.so ../../bin/


