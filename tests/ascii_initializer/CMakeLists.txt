cmake_minimum_required (VERSION 3.2)
project (test-asciiinitializerfactory)


set(SOURCES
    test_asciiinitializerfactory.cpp
    ../test_commons.cpp
    )

include_directories("./")
add_executable(test_asciiinitializer ${SOURCES})
target_link_libraries(test_asciiinitializer gtest gtest_main)
target_link_libraries(test_asciiinitializer gmock gmock_main)
target_link_libraries(test_asciiinitializer phareinitializer pharecore pharedata phareutilities)

add_test(NAME test-asciiinitializerfactory COMMAND test_asciiinitializer)
add_custom_command(TARGET test_asciiinitializer
                   POST_BUILD
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/writeini.py ${CMAKE_CURRENT_BINARY_DIR})


