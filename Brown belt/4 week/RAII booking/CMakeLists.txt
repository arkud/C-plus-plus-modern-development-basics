cmake_minimum_required(VERSION 3.16)
project(RAII_booking)

set(CMAKE_CXX_STANDARD 17)

add_executable(RAII_booking new_booking_providers.h new_trip_manager.h test_runner.h test.cpp booking.h)