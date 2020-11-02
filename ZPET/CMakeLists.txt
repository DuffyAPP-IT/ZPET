cmake_minimum_required(VERSION 3.17)
project(ZPET C)

set(CMAKE_CXX_STANDARD 20)

add_executable(ZPETv2 main.cpp misc.cpp misc.h module.cpp module.hpp processor.cpp processor.hpp)
