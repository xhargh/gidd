cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H -std=c++11")

set(SOURCE_FILES
        gidd.cpp
        gidd_utils.cpp
        file.cpp
        output_generator.cpp
        dot_generator.cpp
        plantuml_generator.cpp
        gml_generator.cpp
        main.cpp)

add_executable(gidd ${SOURCE_FILES})
