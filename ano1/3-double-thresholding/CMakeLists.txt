cmake_minimum_required(VERSION 3.6)
project(3-double-thresholding)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(../0-utils)

add_executable(3-double-thresholding ${SOURCE_FILES})
target_link_libraries(3-double-thresholding ${OpenCV_LIBS})