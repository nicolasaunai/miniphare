cmake_minimum_required (VERSION 3.2)
project (test-Faraday)

set(SOURCES
    test_main.cpp
    test_utilities.cpp
    test_faraday1d.cpp
    )





include_directories("./")
add_executable(Faraday ${SOURCES})
target_link_libraries(Faraday gtest gtest_main)
target_link_libraries(Faraday gmock gmock_main)
target_link_libraries(Faraday libpharecore libpharedata)

add_test(NAME test-Faraday COMMAND Faraday)
add_custom_command(TARGET Faraday
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/faradaytest.py ${CMAKE_CURRENT_BINARY_DIR})


