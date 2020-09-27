cmake_minimum_required(VERSION 3.16)
project(Priority_container)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_CXX_STANDARD 17)

add_executable(Priority_container priority_collection_2.cpp test_runner.h)