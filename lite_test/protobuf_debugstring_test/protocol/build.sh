#!/bin/sh

export PATH=$PATH:../protobuf-2.5.0/bin/

SRC_DIR=.
DST_DIR=../src/
PROTO_DIR=.

protoc -I=$SRC_DIR --cpp_out=$DST_DIR $PROTO_DIR/message.proto
