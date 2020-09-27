cmake_minimum_required(VERSION 3.16)
project(Code_restructure)

set(CMAKE_CXX_STANDARD 17)

add_executable(Code_restructure main.cpp stats_aggregator.cpp stats_aggregator.h test_runner.h stats_aggregator_test.cpp)