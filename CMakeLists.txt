cmake_minimum_required(VERSION 3.0.0)
project(image-loader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wnon-virtual-dtor -Woverloaded-virtual")

set(BUILD_TARGET "test")
add_executable(${BUILD_TARGET})
target_sources(${BUILD_TARGET}
               PRIVATE
               test.cpp
               bmp.h bmp.cpp)
