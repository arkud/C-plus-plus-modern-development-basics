cmake_minimum_required(VERSION 3.16)
project(Final_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Final_project paginator.cpp main.cpp iterator_range.h parse.cpp parse.h search_server.cpp search_server.h test_runner.h)