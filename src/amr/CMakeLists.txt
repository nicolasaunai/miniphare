cmake_minimum_required (VERSION 3.2)
project (libphareamr)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

include_directories("MLMD" "Hierarchy" "Patch" "Refinement")

add_library(libphareamr ${SOURCES})

target_link_libraries(libphareamr)

#add_subdirectory(core)
#add_subdirectory(data)

#include(../cppcheck.cmake)
