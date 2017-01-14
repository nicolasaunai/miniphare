cmake_minimum_required (VERSION 2.6)
project (miniphare)

set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

add_compile_options(-std=c++11 -stdlib=libc++)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include_directories("src")
add_subdirectory(src)



option(test "Build all tests." ON) # Makes boolean 'test' available.


if (test)
  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(googletest)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  #add_subdirectory(tests/Faraday)
  add_subdirectory(tests/GridLayout)
  add_subdirectory(tests/Plasma)

endif()
