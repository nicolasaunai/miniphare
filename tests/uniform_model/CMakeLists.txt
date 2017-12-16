cmake_minimum_required (VERSION 3.2)
project (test-uniformmodel)

set(SOURCES
    test_uniform_model.cpp
    )


include_directories("./")
add_executable(test_uniform_model ${SOURCES})
target_link_libraries(test_uniform_model gtest gtest_main)
target_link_libraries(test_uniform_model gmock gmock_main)
target_link_libraries(test_uniform_model phareinitializer pharedata phareutilities)
add_test(NAME test-uniformmodel COMMAND test_uniform_model)

