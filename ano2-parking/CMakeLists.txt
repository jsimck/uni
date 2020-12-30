cmake_minimum_required(VERSION 3.6)
project(vsb-ano2-parking)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
set(SOURCE_FILES main.cpp core/Place.cpp core/Place.h objdetect/Classifier.cpp objdetect/Classifier.h utils/Loader.cpp utils/Loader.h)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(vsb-ano2-parking ${SOURCE_FILES})
target_link_libraries(vsb-ano2-parking ${OpenCV_LIBS})