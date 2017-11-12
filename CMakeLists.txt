cmake_minimum_required (VERSION 3.2)
project (miniphare)

set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

add_compile_options(-Wall)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif ( MSVC )


find_package( PythonInterp 3.0 REQUIRED )




if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)






option(test "Build all tests." ON) # Makes boolean 'test' available.
option(coverage "Generate code coverage" ON)


if (test)
  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(googletest)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

  if (coverage)
    # coverage
    set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
    #list(APPEND CMAKE_STATIC_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/out/index.html
        COMMAND lcov --capture -directory ${CMAKE_CURRENT_BINARY_DIR} --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
        COMMAND genhtml ${CMAKE_CURRENT_BINARY_DIR}/coverage.info --output-directory ${CMAKE_CURRENT_BINARY_DIR}/out
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating coverage repport"
        )
    add_custom_target(coverage_report DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/out/index.html)
  endif()

  include_directories("src")
  add_subdirectory(src)
  ##############
  # Unit Tests
  ##############
  add_subdirectory(tests/Faraday)
  add_subdirectory(tests/GridLayout)
  add_subdirectory(tests/Pusher)
  add_subdirectory(tests/IndexesAndWeights)
  add_subdirectory(tests/Splitting)
  add_subdirectory(tests/ascii_initializer)
  #add_subdirectory(tests/Plasma)

endif()
