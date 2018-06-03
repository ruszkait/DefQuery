cmake_minimum_required(VERSION 3.1)

set(ProjectName DefQuery)
project(${ProjectName} C CXX)
message(STATUS "Begin Project " ${ProjectName})

get_filename_component(ProjectAbsoluteDir "." ABSOLUTE)
message(STATUS "Project directory " ${ProjectAbsoluteDir})
file(GLOB Sources RELATIVE ${ProjectAbsoluteDir} "*.h" )
message(STATUS "Source files " ${Sources})

add_library(${ProjectName} INTERFACE)

#target_sources(${ProjectName} INTERFACE ${Sources})
message(STATUS "Source files " ${INTERFACE_SOURCES})

target_include_directories(${ProjectName} INTERFACE ${ProjectAbsoluteDir})


message(STATUS "End Project " ${ProjectName})
