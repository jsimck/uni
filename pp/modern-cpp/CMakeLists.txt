cmake_minimum_required(VERSION 3.6)
project(modern-cpp)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp -Wall -pedantic")

set(SOURCE_FILES main.cpp test.h multithreading.h)

add_executable(modern-cpp ${SOURCE_FILES})