cmake_minimum_required(VERSION 3.6)
project(vsb-dzo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add project subdirectories
add_subdirectory(1-basics)
add_subdirectory(2-gamma-contrast-correction)
add_subdirectory(3-convolution)
add_subdirectory(4-anisotropic-filter)
add_subdirectory(5-discrete-fourier-transform)
add_subdirectory(6-inverse-discrete-fourier-transform)
add_subdirectory(7-dft-filters)
add_subdirectory(8-geometric-distortion)
add_subdirectory(9-histogram)
add_subdirectory(10-perspective-transform)
add_subdirectory(11-retinex)