cmake_minimum_required(VERSION 3.10)
project(AoC-2020)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(aoclib)
add_library(aoclib aoclib/aoclib.cpp)

foreach(D RANGE 1 9)
    message(STATUS "day-0${D}")
    add_executable("day-0${D}" "day-0${D}/solution.cpp")
    target_link_libraries("day-0${D}" aoclib)
endforeach()
