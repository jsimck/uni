cmake_minimum_required(VERSION 3.6)
project(vsb-pa-lu-decomposition)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -Wall -pedantic")
set(SOURCE_FILES main.cpp types/mat.cpp types/mat.h algorithm/lu_open_mp.cpp algorithm/lu_open_mp.h utils/timer.cpp utils/timer.h utils/usings.h
        algorithm/lu_cpp_11.h algorithm/lu_cpp_11.cpp algorithm/lu.h)

add_executable(vsb-pa-lu-decomposition ${SOURCE_FILES})