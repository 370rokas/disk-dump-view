cmake_minimum_required(VERSION 3.10)

project(DiskDumpView)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

add_executable(DiskDumpView
    src/main.cpp
    src/partTables/partitionTables.cpp
    src/partTables/mbrTable.cpp
    src/partTables/gptTable.cpp
)