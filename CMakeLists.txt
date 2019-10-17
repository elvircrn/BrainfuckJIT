cmake_minimum_required(VERSION 3.15)
project(BrainfuckJIT)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")


set(CMAKE_CXX_FLAGS_DEBUG "-g")
#TODO: Figure out why this code crashes on O1/2/3
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -flto -fno-inline")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -flto")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")

file(GLOB_RECURSE source_files
        "include/*.h"
        "src/*.cpp"
        "asmjit/*.cpp"
        main.cpp)

#set library
add_library(STATIC ${source_files})
include_directories(include asmjit)

add_executable(BrainfuckJIT ${source_files})
target_link_libraries(BrainfuckJIT -lrt ~/CLionProjects/asmjit/cmake-build-debug/libasmjit.so)
