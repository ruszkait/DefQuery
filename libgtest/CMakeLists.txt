cmake_minimum_required(VERSION 3.1)

set(ProjectName libgtest)
project(${ProjectName} C CXX)
message(STATUS "Begin Project " ${ProjectName})

#find_package(Threads REQUIRED)

get_filename_component(ProjectAbsoluteDir "." ABSOLUTE)
message(STATUS "Project directory " ${ProjectAbsoluteDir})
file(GLOB Sources RELATIVE ${ProjectAbsoluteDir} "src/*.cc" )
message(STATUS "Source files " ${Sources})

add_library(${ProjectName} STATIC ${Sources})

# Header files internal to this target
target_include_directories(${ProjectName} PRIVATE ${ProjectAbsoluteDir})

# Header files for external consumers of this target
target_include_directories(${ProjectName} PUBLIC  ${ProjectAbsoluteDir}/include)

message(STATUS "End Project " ${ProjectName})