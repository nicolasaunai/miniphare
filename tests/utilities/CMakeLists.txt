cmake_minimum_required (VERSION 3.2)
project (test-utilities)

set(SOURCES
    test_utilities.cpp
    )


include_directories("./")
add_executable(test_utilities ${SOURCES})
target_link_libraries(test_utilities gtest gtest_main)
target_link_libraries(test_utilities gmock gmock_main)
target_link_libraries(test_utilities phareutilities)
add_test(NAME test-utilities COMMAND test_utilities)


