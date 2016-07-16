project(finstrument_mock CXX)
cmake_minimum_required(VERSION 3.2)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_subdirectory(compiler-rt)
add_subdirectory(test)
