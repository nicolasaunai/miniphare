cmake_minimum_required (VERSION 2.6)
project (test-Plasmas)

set(SOURCES
    test_ions.cpp
       test_species.cpp
     ../../src/Plasmas/ions.cpp
     ../../src/grid/gridlayout.cpp
     ../../src/grid/gridlayoutimplinternals.cpp
     ../../src/grid/gridlayoutimplyee.cpp
     ../../src/Plasmas/species.cpp
      ../../src/vecfield/vecfield.cpp
      ../../src/Field/field.cpp
      ../../src/Projector/projector.cpp
      ../../src/IndexesAndWeights/indexesandweights.cpp
      ../../src/IndexesAndWeights/indexesandweightso1.cpp
      ../../src/IndexesAndWeights/indexesandweightso2.cpp
      ../../src/IndexesAndWeights/indexesandweightso3.cpp
      ../../src/IndexesAndWeights/indexesandweightso4.cpp
      test_ions_chargedensity.cpp)


include_directories("./")
add_executable(Plasma ${SOURCES})
target_link_libraries(Plasma gtest gtest_main)
target_link_libraries(Plasma gmock gmock_main)
add_test(NAME test-Plasmas  COMMAND Plasma)
add_custom_command(TARGET Gridlayout
                   POST_BUILD
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/gridlayouttest.py ${CMAKE_CURRENT_BINARY_DIR})

