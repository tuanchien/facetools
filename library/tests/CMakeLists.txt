#
# This is a CMake makefile.  You can find the cmake utility and
# information about it at http://www.cmake.org
#

cmake_minimum_required(VERSION 2.8)

PROJECT(facetools_validation)

set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_FLAGS "-std=c++14 -O3")

file(GLOB SOURCES src/*.cpp)

set(LINK_LIBRARIES
facetools
dlib
openblas
gtest
gtest_main
pthread
)

set(LINK_DIRECTORIES
${CMAKE_SOURCE_DIR}
)

set(INCLUDES
${CMAKE_SOURCE_DIR}/library/include
)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(${INCLUDES})

add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/gtest/libgtest.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/gtest/include"
)

add_library(gtest_main STATIC IMPORTED)
set_target_properties(gtest_main PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/gtest/libgtest_main.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/gtest/include"
)

add_library(facetools STATIC IMPORTED)
set_target_properties(facetools PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/build/library/libfacetools.a"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/include"
)

add_executable(facetools_validation ${SOURCES})
link_directories(${LINK_DIRECTORIES})
target_link_libraries (facetools_validation LINK_PUBLIC ${LINK_LIBRARIES})
add_dependencies(facetools_validation facetools_static)
