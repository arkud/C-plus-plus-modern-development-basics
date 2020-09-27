cmake_minimum_required(VERSION 3.16)
project(Cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Cache main.cpp Common.h Solution.cpp test_runner.h)