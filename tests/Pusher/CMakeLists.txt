cmake_minimum_required (VERSION 3.2)
project (test-Pusher)

set(SOURCES
     test_main.cpp
     test_utilities.cpp
     test_commons.cpp
     test_pusher1d.cpp
    )





include_directories("./")
add_executable(Pusher ${SOURCES})
target_link_libraries(Pusher gtest gtest_main)
target_link_libraries(Pusher gmock gmock_main)
target_link_libraries(Pusher pharecore pharedata phareutilities)

add_test(NAME test-Pusher COMMAND Pusher)
add_custom_command(TARGET Pusher
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/test_pusher.py ${CMAKE_CURRENT_BINARY_DIR})


