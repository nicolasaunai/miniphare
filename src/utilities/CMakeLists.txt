cmake_minimum_required (VERSION 3.2)
project (libphareutilities)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

include_directories("Time")

add_library(libphareutilities ${SOURCES})

#add_subdirectory(core)
#add_subdirectory(data)

#include(../cppcheck.cmake)
