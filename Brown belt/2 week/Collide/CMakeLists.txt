cmake_minimum_required(VERSION 3.16)
project(Collide)

set(CMAKE_CXX_STANDARD 17)

add_executable(Collide collide.cpp game_object.h geo2d.cpp geo2d.h test_runner.h)