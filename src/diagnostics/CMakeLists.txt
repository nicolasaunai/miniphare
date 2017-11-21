cmake_minimum_required (VERSION 3.2)
project (pharediagnostics)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

include_directories("Export" "FieldDiagnostics")
add_library(pharediagnostics ${SOURCES})
target_link_libraries(pharediagnostics)

