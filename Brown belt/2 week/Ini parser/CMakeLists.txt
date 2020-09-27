cmake_minimum_required(VERSION 3.16)
project(Ini_parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ini_parser test_ini.cpp ini.h test_runner.h ini.cpp)