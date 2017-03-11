cmake_minimum_required (VERSION 2.6)
project (miniphare)

find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)


add_executable(miniphare ${SOURCES})

include(../cppcheck.cmake)




