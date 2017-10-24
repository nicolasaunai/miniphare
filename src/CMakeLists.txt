cmake_minimum_required (VERSION 3.2)
project (miniphare)

#find_package(MPI)


add_executable(miniphare main.cpp)#${SOURCES})

add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(utilities)
add_subdirectory(amr)
add_subdirectory(diagnostics)
add_subdirectory(initializer)


include_directories("core" "data" "utilities")

target_link_libraries(miniphare libpharediagnostics libphareamr libpharecore libphareinitializer libphareutilities libpharedata)


include(../cppcheck.cmake)




