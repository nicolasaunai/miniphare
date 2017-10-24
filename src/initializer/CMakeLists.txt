cmake_minimum_required (VERSION 3.2)
project (libphareinitializer)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

#include_directories("Electromag" "Field" "grid" "Plasmas" "vecfield")

add_library(libphareinitializer ${SOURCES})

#add_subdirectory(core)
#add_subdirectory(data)

#include(../cppcheck.cmake)
