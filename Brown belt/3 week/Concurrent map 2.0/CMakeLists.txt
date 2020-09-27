cmake_minimum_required(VERSION 3.16)
project(Concurrent_map_2_0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Concurrent_map_2_0 concurrent_map.cpp profile.h test_runner.h)