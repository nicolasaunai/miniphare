cmake_minimum_required (VERSION 3.2)
project (phareinitializer)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)
add_library(phareinitializer ${SOURCES})
