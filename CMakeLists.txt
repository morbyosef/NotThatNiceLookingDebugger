cmake_minimum_required (VERSION 3.0)
project(Debugger)

include_directories(ext/libelfin ext/linenoise)
set(CMAKE_CXX_STANDARD 14)

add_executable(Debugger ./src/main.cpp ./src/debugger.cpp ./src/debugger.h ./src/Helper.cpp ./src/Helper.h ./src/Breakpoint.cpp ./src/Breakpoint.h ./src/Registers.h ./src/Registers.cpp ./ext/linenoise/linenoise.c)

add_custom_target(
   libelfin
   COMMAND make
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin
)

target_link_libraries(Debugger
                      ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
                      ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so)

add_dependencies(Debugger libelfin)