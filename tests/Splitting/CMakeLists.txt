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
     ../../src/Splitting/splittingstrategy.cpp
     ../../src/Splitting/approx_1to4strategy.cpp
     ../../src/Splitting/approx_fujistrategy.cpp
     ../../src/Splitting/order1_rf2strategy.cpp
     ../../src/Splitting/order2_rf2strategy.cpp
     ../../src/Splitting/order3_rf2strategy.cpp
     ../../src/Splitting/order1_rfnstrategy.cpp
     ../../src/Splitting/ordern_rf2strategy.cpp
     ../../src/Splitting/ordern_rf3strategy.cpp
     ../../src/Splitting/multinomial.cpp
     test_exactsplitting1part.cpp
     test_approxsplitting1part.cpp
     test_utilities.cpp
     test_commons.cpp
     test_main.cpp
    )



include_directories("./")
add_executable(Splitting ${SOURCES})
target_link_libraries(Splitting gtest gtest_main)
target_link_libraries(Splitting gmock gmock_main)
add_test(NAME test-Splitting COMMAND test-Splitting)



