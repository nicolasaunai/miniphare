cmake_minimum_required (VERSION 3.2)
project (test-indexweight)

set(SOURCES
    test_main.cpp
    test_utilities.cpp
    test_weightsum.cpp
    test_indexes.cpp
    test_exactweights.cpp
    )





include_directories("./")
add_executable(IndexesAndWeight ${SOURCES})
target_link_libraries(IndexesAndWeight gtest gtest_main)
target_link_libraries(IndexesAndWeight gmock gmock_main)
target_link_libraries(IndexesAndWeight libpharecore libpharedata)

add_test(NAME test-indexweight COMMAND IndexesAndWeight)
add_custom_command(TARGET IndexesAndWeight
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/test_IndexesAndWeights.py ${CMAKE_CURRENT_BINARY_DIR})
