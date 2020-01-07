cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H -std=c++11")

set(SOURCE_FILES
        gidd.cpp
        gidd_utils.cpp
        file.cpp
        output_generators/output_generator.cpp
        output_generators/dot_generator.cpp
        output_generators/plantuml_generator.cpp
        output_generators/gml_generator.cpp
        main.cpp)

add_executable(gidd ${SOURCE_FILES})
