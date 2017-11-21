cmake_minimum_required (VERSION 3.2)
project (pharecore)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

add_library(pharecore ${SOURCES})
target_link_libraries(pharecore pharedata)

include_directories("core" "data/grid/" "data/Field")


#add_subdirectory(core)
#add_subdirectory(data)

#include(../cppcheck.cmake)
