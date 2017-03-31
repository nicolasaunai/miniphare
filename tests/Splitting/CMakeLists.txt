cmake_minimum_required (VERSION 2.6)
project (test-Splitting)

set(SOURCES
     ../../src/Plasmas/particles.cpp
     ../../src/Interpolator/interpolator.cpp
     ../../src/Interpolator/particlemesh.cpp
     ../../src/grid/gridlayout.cpp
     ../../src/grid/gridlayoutimplyee.cpp
     ../../src/grid/gridlayoutimplinternals.cpp
     ../../src/vecfield/vecfield.cpp
     ../../src/Field/field.cpp
     test_splitting1part.cpp
     test_utilities.cpp
     test_commons.cpp
     test_main.cpp
    )



include_directories("./")
add_executable(Splitting ${SOURCES})
target_link_libraries(Splitting gtest gtest_main)
target_link_libraries(Splitting gmock gmock_main)
add_test(NAME test-Splitting COMMAND Gridlayout)


