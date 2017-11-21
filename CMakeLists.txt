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
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info
      COMMAND lcov --capture --no-external --initial --directory ${CMAKE_CURRENT_SOURCE_DIR} --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Pre-coverage report"
     )
     add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/out/index.html
       COMMAND lcov --capture --no-external --directory ${CMAKE_CURRENT_SOURCE_DIR} --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage-test.info
       COMMAND lcov --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info --add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage-test.info
       --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
       COMMAND lcov --remove ${CMAKE_CURRENT_BINARY_DIR}/coverage.info '/usr/include/*' 'googletest/*' 'tests/*' -o ${CMAKE_CURRENT_BINARY_DIR}/filtered.info
       COMMAND genhtml --ignore-errors source ${CMAKE_CURRENT_BINARY_DIR}/filtered.info
       --prefix ${CMAKE_CURRENT_SOURCE_DIR} --keep-descriptions --frames --show-details --legend
       --output-directory ${CMAKE_CURRENT_BINARY_DIR}/out
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating coverage repport"
       )
    add_custom_target(pre_coverage DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/coverage-base.info)
    add_custom_target(coverage_report DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/out/index.html)
  endif()
endif()

include_directories("src")
add_subdirectory(src)

if (test)
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
