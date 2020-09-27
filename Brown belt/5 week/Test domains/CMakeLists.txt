cmake_minimum_required(VERSION 3.16)
project(Test_domains)

set(CMAKE_CXX_STANDARD 17)

add_executable(Test_domains test_runner.h test_domains.cpp)