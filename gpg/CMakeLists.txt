cmake_minimum_required(VERSION 3.6)
project(vsb-gpg)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-6") # Use your own path to gcc6 compiler
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-6") # Use your own path to gcc6 compiler
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp geom/material.cpp geom/material.h geom/objloader.cpp geom/objloader.h geom/omnilight.cpp geom/omnilight.h geom/ray.cpp geom/ray.h geom/surface.cpp geom/surface.h geom/texture.cpp geom/texture.h geom/triangle.cpp geom/triangle.h geom/vertex.cpp geom/vertex.h math/color4.cpp math/color4.h math/matrix4x4.cpp math/matrix4x4.h math/quaternion.cpp math/quaternion.h math/vector2.cpp math/vector2.h math/vector3.cpp math/vector3.h math/vector4.cpp math/vector4.h tracing/camera.cpp tracing/camera.h utils/utils.cpp utils/utils.h embree/errorchecks.h embree/errorchecks.cpp embree/structs.h utils/macros.h utils/filters.cpp tracing/cubemap.h tracing/cubemap.cpp shaders/shader.h shaders/shader.cpp shaders/shaderlambert.cpp shaders/shaderlambert.h shaders/shadernormal.cpp shaders/shadernormal.h tracing/renderer.cpp tracing/renderer.h shaders/shaderphong.cpp shaders/shaderphong.h utils/timer.cpp utils/timer.h shaders/shaderpathtracer.cpp shaders/shaderpathtracer.h tracing/intersect.cpp tracing/intersect.h)

find_package(Embree 2.14.0 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${EMBREE_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(utils)
include_directories(embree)
include_directories(math)
include_directories(shaders)
include_directories(geom)
include_directories(tracing)

add_executable(vsb-gpg ${SOURCE_FILES})
target_link_libraries(vsb-gpg ${OpenCV_LIBS} ${EMBREE_LIBRARY})