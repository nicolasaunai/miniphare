cmake_minimum_required (VERSION 2.6)
project (test-Pusher)

set(SOURCES
     ../../src/Plasmas/particles.cpp
     ../../src/Interpolator/interpolator.cpp
     ../../src/Interpolator/particlemesh.cpp
     ../../src/pusher/modifiedboris.cpp
     ../../src/grid/gridlayout.cpp
     ../../src/grid/gridlayoutimplyee.cpp
     ../../src/grid/gridlayoutimplinternals.cpp
     ../../src/BoundaryConditions/boundary_conditions.cpp
     ../../src/BoundaryConditions/domainboundarycondition.cpp
     ../../src/BoundaryConditions/periodicdomainboundary.cpp
     ../../src/vecfield/vecfield.cpp
     ../../src/Field/field.cpp
     ../../src/BoundaryConditions/domainboundarycondition.cpp
     ../../src/BoundaryConditions/periodicdomainboundary.cpp
     test_main.cpp
     test_utilities.cpp
     test_commons.cpp
     test_pusher1d.cpp
    )





include_directories("./")
add_executable(Pusher ${SOURCES})
target_link_libraries(Pusher gtest gtest_main)
target_link_libraries(Pusher gmock gmock_main)
add_test(NAME test-Pusher COMMAND Gridlayout)
add_custom_command(TARGET Pusher
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/test_pusher.py ${CMAKE_CURRENT_BINARY_DIR})


