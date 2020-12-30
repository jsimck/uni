cmake_minimum_required(VERSION 3.6)
project(1-object-detection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(../0-utils)

add_executable(1-object-detection ${SOURCE_FILES})
target_link_libraries(1-object-detection ${OpenCV_LIBS})