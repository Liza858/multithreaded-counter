cmake_minimum_required(VERSION 3.16)
project(counter)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(untitled counter.hpp test.cpp)
