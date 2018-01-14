#!/bin/sh
protoc -I=./ --cpp_out=./ types.proto
mv types.pb.h types.pb.hh
sed '5s/.*/#include "types.pb.hh"/' types.pb.cc > tmp-types.cc
rm types.pb.cc
mv tmp-types.cc types.cc
