cmake_minimum_required(VERSION 3.6)
project(vsb-agu)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -Wall -pedantic")
set(SOURCE_FILES main.cpp types/polygon.cpp types/polygon.h types/line.h types/line.cpp excercises/aexercise.h
        excercises/e1_polygons.cpp excercises/e1_polygons.h
        collections/dictionary.cpp collections/dictionary.h collections/priority_queue.h types/event_point.cpp types/event_point.h
        types/segment.h types/segment.cpp algorithms/plane_sweeping.h algorithms/plane_sweeping.cpp excercises/e2_plane_sweeping.cpp excercises/e2_plane_sweeping.h
        utils/visualizer.h utils/visualizer.cpp utils/generator.cpp utils/generator.h utils/utils.cpp utils/utils.h excercises/e3_convex_hull.cpp excercises/e3_convex_hull.h algorithms/convex_hull.cpp algorithms/convex_hull.h types/)

add_executable(vsb-agu ${SOURCE_FILES})

find_package(PkgConfig)
pkg_search_module(OpenCV REQUIRED opencv)

target_include_directories(vsb-agu SYSTEM PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(vsb-agu ${OpenCV_LIBRARIES})