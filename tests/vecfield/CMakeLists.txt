cmake_minimum_required (VERSION 3.2)
project (test-vecfieldoperations)

set(SOURCES
    test_vecfieldoperations.cpp
    test_commons.cpp
    )


include_directories("./")
add_executable(test_vecfieldoperations ${SOURCES})
target_link_libraries(test_vecfieldoperations gtest gtest_main)
target_link_libraries(test_vecfieldoperations gmock gmock_main)
target_link_libraries(test_vecfieldoperations phareinitializer pharecore pharedata phareutilities)
add_test(NAME test-vecfieldoperations COMMAND test_vecfieldoperations)

