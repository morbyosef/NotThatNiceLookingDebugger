# cmake_minimum_required(VERSION <specify CMake version here>)
project(Debugger)

set(CMAKE_CXX_STANDARD 14)

add_executable(Debugger ./src/main.cpp ./src/debugger.cpp ./src/debugger.h ./src/linenoise.h ./src/linenoise.c ./src/Helper.cpp ./src/Helper.h ./src/Breakpoint.cpp ./src/Breakpoint.h ./src/Registers.h ./src/Registers.cpp)