cmake_minimum_required (VERSION 3.2)
project (phareutilities)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

include_directories("Time")

add_library(phareutilities ${SOURCES})
