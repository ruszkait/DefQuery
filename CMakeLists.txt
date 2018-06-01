cmake_minimum_required(VERSION 3.1)

set(ProjectName DefQuery)
project(${ProjectName} C CXX)
message(STATUS "Begin Project container " ${ProjectName})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BinaryDirectory "${PROJECT_SOURCE_DIR}/bin/x64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BinaryDirectory}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BinaryDirectory}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BinaryDirectory}")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${BinaryDirectory}")
message(STATUS "Output paths were set to " ${BinaryDirectory})

message(STATUS "Adding sub projects")
add_subdirectory(DefQueryTest "${CMAKE_BINARY_DIR}/DefQueryTest")
add_subdirectory(DefQuery "${CMAKE_BINARY_DIR}/DefQuery")
add_subdirectory(libgtest "${CMAKE_BINARY_DIR}/libgtest")

message(STATUS "End Project container " ${ProjectName})
