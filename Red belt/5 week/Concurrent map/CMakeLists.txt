cmake_minimum_required(VERSION 3.16)
project(Concurrent_map)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Concurrent_map concurrent_map.cpp test_runner.h profile.h)