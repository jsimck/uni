cmake_minimum_required(VERSION 3.6)
project(vsb-vd)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -Wall -pedantic")
set(SOURCE_FILES main.cpp exercises/e1_cloud_scatter.cpp exercises/e1_cloud_scatter.h exercises/aexercise.h utils/utils.cpp utils/utils.h exercises/e2_flow.cpp exercises/e2_flow.h exercises/e3_depth.cpp exercises/e3_depth.h core/synthetic_scene.cpp core/synthetic_scene.h)

add_executable(vsb-vd ${SOURCE_FILES})

find_package(PkgConfig)
pkg_search_module(OpenCV REQUIRED opencv)

target_include_directories(vsb-vd SYSTEM PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(vsb-vd ${OpenCV_LIBRARIES})