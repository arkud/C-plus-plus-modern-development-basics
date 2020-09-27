cmake_minimum_required(VERSION 3.16)
project(Expression_tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(Expression_tree main.cpp Common.h test_runner.h expression.cpp)