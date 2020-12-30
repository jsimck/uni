cmake_minimum_required(VERSION 3.6)
project(matrix-multiplication)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -Wall -pedantic")

set(SOURCE_FILES main.cpp multiplication.cpp multiplication.h mat.cpp mat.h timer.cpp timer.h)

add_executable(matrix-multiplication ${SOURCE_FILES})