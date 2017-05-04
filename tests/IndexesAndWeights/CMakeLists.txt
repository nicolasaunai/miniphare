cmake_minimum_required (VERSION 2.6)
project (test-indexweight)

set(SOURCES
    ../../src/grid/gridlayout.cpp
    ../../src/grid/gridlayoutimplyee.cpp
    ../../src/grid/gridlayoutimplinternals.cpp
    ../../src/IndexesAndWeights/indexesandweights.cpp
    ../../src/Plasmas/particles.cpp
    test_main.cpp
    test_utilities.cpp
    test_weights.cpp
    test_indexes.cpp
    )





include_directories("./")
add_executable(IndexesAndWeight ${SOURCES})
target_link_libraries(IndexesAndWeight gtest gtest_main)
target_link_libraries(IndexesAndWeight gmock gmock_main)
add_test(NAME test-indexweight COMMAND IndexesAndWeight)
add_custom_command(TARGET IndexesAndWeight
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/test_IndexesAndWeights.py ${CMAKE_CURRENT_BINARY_DIR})
