#!/bin/sh

protoc  -I=./ --python_out=./ ./protocol/im.proto
