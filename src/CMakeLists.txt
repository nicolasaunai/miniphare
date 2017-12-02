cmake_minimum_required (VERSION 3.2)
project (miniphare)

#find_package(MPI)


#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/gitsha1.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/gitsha1.cpp" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/gitsha1.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/gitsha1.cpp" @ONLY)
set (SOURCES "${CMAKE_CURRENT_BINARY_DIR}/gitsha1.cpp"  main.cpp gitsha1.h)

#list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/gitsha1.cpp" ${CMAKE_CURRENT_SOURCE_DIR}/gitsha1.h)

add_executable(miniphare ${SOURCES})

add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(utilities)
add_subdirectory(amr)
add_subdirectory(diagnostics)
add_subdirectory(initializer)


include_directories("core" "data" "utilities")

target_link_libraries(miniphare pharediagnostics phareamr phareinitializer pharecore pharedata phareutilities )


include(../cppcheck.cmake)




