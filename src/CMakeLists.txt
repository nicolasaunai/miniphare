cmake_minimum_required (VERSION 2.6)
project (miniphare)

find_package(MPI)
file(GLOB_RECURSE SOURCES src *.cpp *.h)


add_executable(miniphare ${SOURCES})



find_program(CPPCHECK cppcheck)

add_custom_command( TARGET miniphare
    POST_BUILD
    COMMAND ${CPPCHECK} #/opt/local/bin/cppcheck
    --enable=all#warning,performance,portability,information,missingInclude
    --std=c++11
    --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
    --verbose
    --quiet
   ${SOURCES}
   )



