cmake_minimum_required(VERSION 3.6)
project(4-simple-obj-classifier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp structs/color.h structs/color.cpp structs/shape.cpp structs/shape.h classifier.h classifier.cpp structs/group.h structs/group.cpp)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(../0-utils)

add_executable(4-simple-obj-classifier ${SOURCE_FILES})
target_link_libraries(4-simple-obj-classifier ${OpenCV_LIBS})