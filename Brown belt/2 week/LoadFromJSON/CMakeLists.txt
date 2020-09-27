cmake_minimum_required(VERSION 3.16)
project(LoadFromJSON)

set(CMAKE_CXX_STANDARD 17)

add_executable(LoadFromJSON spendings_json.cpp json.cpp json.h test_runner.h)