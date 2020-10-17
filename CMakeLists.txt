cmake_minimum_required(VERSION 3.17)
project(ZPETv2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ZPETv2 main.cpp module.cpp module.h misc.cpp misc.h processor.cpp processor.h)