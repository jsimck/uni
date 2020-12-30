cmake_minimum_required(VERSION 3.6)
project(3-convolution)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp convolution.h)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(3-convolution ${SOURCE_FILES})
target_link_libraries(3-convolution ${OpenCV_LIBS})
