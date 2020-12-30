cmake_minimum_required(VERSION 3.6)
project(vsb-ano)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add project subdirectories
add_subdirectory(1-derivatives)
add_subdirectory(2-laplace)
add_subdirectory(3-double-thresholding)
add_subdirectory(4-simple-obj-classifier)
add_subdirectory(5-mog)