cmake_minimum_required(VERSION 3.5)
project(gidd)

include_directories(inc/../inc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H -std=c++11")

set(SOURCE_FILES gidd.cpp)
add_executable(gidd ${SOURCE_FILES})

