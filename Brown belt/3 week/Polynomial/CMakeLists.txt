cmake_minimum_required(VERSION 3.16)
project(Polynomial)

set(CMAKE_CXX_STANDARD 17)

add_executable(Polynomial polynomial.cpp test_runner.h profile.h)