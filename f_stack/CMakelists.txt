cmake_minimum_required(VERSION 3.0)
project(FTSFC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")

add_executable(Server
        server.cc
        server.hh)


