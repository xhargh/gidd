cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES gidd_test.cpp ../src/gidd_utils.cpp)

include(CTest)

add_executable(gidd_test ${SOURCE_FILES})
add_test( NAME testname COMMAND gidd_test )
