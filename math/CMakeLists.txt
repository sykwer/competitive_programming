cmake_minimum_required(VERSION 3.10)

add_library(Math STATIC
    gcd.cpp
)

add_executable(gcd gcd.cpp)
