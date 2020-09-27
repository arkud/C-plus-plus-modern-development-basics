cmake_minimum_required(VERSION 3.16)
project(Explore_keywords)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Explore_keywords explore_key_words.cpp)