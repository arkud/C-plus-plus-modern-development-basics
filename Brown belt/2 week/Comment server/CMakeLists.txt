cmake_minimum_required(VERSION 3.16)
project(Comment_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(Comment_server comment_server.cpp test_runner.h)