cmake_minimum_required(VERSION 3.16)
project(Bad_macros)

set(CMAKE_CXX_STANDARD 17)

add_executable(Bad_macros test_runner.h print_values.cpp)