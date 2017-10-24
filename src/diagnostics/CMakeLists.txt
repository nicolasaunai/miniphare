cmake_minimum_required (VERSION 3.2)
project (libpharediagnostics)

#find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)

include_directories("Export" "FieldDiagnostics")
#include_directories("Electromag" "Field" "grid" "Plasmas" "vecfield")

add_library(libpharediagnostics ${SOURCES})

target_link_libraries(libpharediagnostics)# libphareamr libpharedata libphareutilities)

#add_subdirectory(core)
#add_subdirectory(data)

#include(../cppcheck.cmake)
