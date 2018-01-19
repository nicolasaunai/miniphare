cmake_minimum_required (VERSION 3.2)
project (test-Splitting)

set(SOURCES
     test_exactsplitting1part.cpp
     test_approxsplitting1part.cpp
     test_utilities.cpp
     test_main.cpp
     ../test_commons.cpp
    )



include_directories("./")
add_executable(Splitting ${SOURCES})
target_link_libraries(Splitting gtest gtest_main)
target_link_libraries(Splitting gmock gmock_main)
target_link_libraries(Splitting pharecore phareamr pharedata)
add_test(NAME test-Splitting COMMAND Splitting)



