cmake_minimum_required(VERSION 3.16)
project(LoadFromXML)

set(CMAKE_CXX_STANDARD 17)

add_executable(LoadFromXML spendings_xml.cpp xml.cpp xml.h test_runner.h)