cmake_minimum_required (VERSION 2.6)
project (test-Faraday)

set(SOURCES
 	../../src/Faraday/faraday.cpp
    ../../src/Faraday/faradayimpl1d.cpp
    ../../src/grid/gridlayout.cpp
    ../../src/grid/gridlayoutimplyee.cpp
    ../../src/grid/gridlayoutimplinternals.cpp
    ../../src/Field/field.cpp 
    ../../src/vecfield/vecfield.cpp 
    test_main.cpp
    test_utilities.cpp
    test_faraday1d.cpp
    )





include_directories("./")
add_executable(Faraday ${SOURCES})
target_link_libraries(Faraday gtest gtest_main)
target_link_libraries(Faraday gmock gmock_main)
add_test(NAME test-Faraday COMMAND Gridlayout)
add_custom_command(TARGET Faraday
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/faradaytest.py ${CMAKE_CURRENT_BINARY_DIR})


