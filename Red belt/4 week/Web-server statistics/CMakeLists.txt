cmake_minimum_required(VERSION 3.16)
project(Web_server_statistics)

set(CMAKE_CXX_STANDARD 17)

add_executable(Web_server_statistics http_request.h server_stats.cpp stats.h stats.cpp)