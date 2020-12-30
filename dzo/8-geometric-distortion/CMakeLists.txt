cmake_minimum_required(VERSION 3.6)
project(8-geometric-distortion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(8-geometric-distortion ${SOURCE_FILES})
target_link_libraries(8-geometric-distortion ${OpenCV_LIBS})