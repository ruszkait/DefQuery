cmake_minimum_required(VERSION 3.1)

set(ProjectName DefQueryTest)
project(${ProjectName} C CXX)
message(STATUS "Begin Project " ${ProjectName})

get_filename_component(ProjectAbsoluteDir "." ABSOLUTE)
message(STATUS "Project directory " ${ProjectAbsoluteDir})
file(GLOB Sources RELATIVE ${ProjectAbsoluteDir} "*.cpp" "*.h" )
message(STATUS "Source files " ${Sources})

add_executable(${ProjectName} ${Sources})

target_link_libraries(${ProjectName} libgtest DefQuery)

message(STATUS "End Project " ${ProjectName})
