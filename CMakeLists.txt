cmake_minimum_required(VERSION 3.10)
project(competitive_programming)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(math)
add_subdirectory(graph)
add_subdirectory(data_structure)
add_subdirectory(segtree)
add_subdirectory(string)
add_subdirectory(flow)

add_executable(template template.cpp)
