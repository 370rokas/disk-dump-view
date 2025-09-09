cmake_minimum_required(VERSION 3.10)

project(DiskDumpView)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DiskDumpView
    src/main.cpp
    src/partitionSchemes.cpp    
)