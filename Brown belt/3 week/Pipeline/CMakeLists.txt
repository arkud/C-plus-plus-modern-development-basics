cmake_minimum_required(VERSION 3.16)
project(Pipeline)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pipeline pipeline.cpp test_runner.h)