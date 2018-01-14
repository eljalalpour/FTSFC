#!/bin/sh
protoc -I=./ --cpp_out=./ types.proto
mv types.pb.h types.pb.hh
mv types.pb.cc types.cc

