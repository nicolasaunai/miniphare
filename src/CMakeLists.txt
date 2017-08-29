cmake_minimum_required (VERSION 3.2)
project (miniphare)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)


add_executable(miniphare ${SOURCES})

include(../cppcheck.cmake)




