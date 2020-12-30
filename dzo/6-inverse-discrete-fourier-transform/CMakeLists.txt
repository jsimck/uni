cmake_minimum_required(VERSION 3.6)
project(6-inverse-discrete-fourier-transform)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp dft.h dft.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(6-inverse-discrete-fourier-transform ${SOURCE_FILES})
target_link_libraries(6-inverse-discrete-fourier-transform ${OpenCV_LIBS})