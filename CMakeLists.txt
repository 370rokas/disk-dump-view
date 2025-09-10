cmake_minimum_required(VERSION 3.10)

project(DiskDumpView)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/main.cpp
    src/fs/ext4.cpp
    src/fs/fsMagicNums.cpp
    src/partTables/partitionTables.cpp
    src/partTables/mbrTable.cpp
    src/partTables/gptTable.cpp
)

add_executable(DiskDumpView ${SOURCES})