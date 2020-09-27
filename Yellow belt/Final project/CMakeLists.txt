cmake_minimum_required(VERSION 3.16)
project(Final_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Final_project main.cpp condition_parser.cpp condition_parser_test.cpp test_runner.h test_runner.cpp
        token.cpp token.h condition_parser.h node.h database.h date.h database.cpp node.cpp date.cpp tests.h)