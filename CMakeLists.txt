cmake_minimum_required(VERSION 3.15)
project(BrainfuckJIT)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")


#set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -flto -fno-inline")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -flto")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")

file(GLOB source_files
        "include/*.h"
        "src/*.cpp"
        "asmjit/*.cpp"
        "asmjit/*.h"
        main.cpp)

#set library
add_library(STATIC ${source_files})
include_directories(include)

add_executable(BrainfuckJIT ${source_files})
