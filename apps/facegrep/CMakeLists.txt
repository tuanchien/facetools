#
# This is a CMake makefile.  You can find the cmake utility and
# information about it at http://www.cmake.org
#

cmake_minimum_required(VERSION 2.8)

PROJECT(facegrep)
set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_FLAGS "-std=c++14 -O3")

file(GLOB LIBRARY_SOURCES src/*.cpp)
file(GLOB BINARY_SOURCES src/*.cpp)

list(REMOVE_ITEM LIBRARY_SOURCES ${CMAKE_SOURCE_DIR}/apps/facegrep/src/main.cpp)

set(LINK_LIBRARIES
facetools
dlib
openblas
)

set(LINK_DIRECTORIES
${CMAKE_SOURCE_DIR}
)

set(INCLUDES
${CMAKE_SOURCE_DIR}/apps/facegrep/include
${CMAKE_SOURCE_DIR}/library/include
)


include_directories(${INCLUDES})

add_library(facetools SHARED IMPORTED)
add_library(facegrep_library STATIC ${LIBRARY_SOURCES})
set_target_properties(facegrep_library PROPERTIES OUTPUT_NAME facegrep)
add_dependencies(facegrep_library facetools)

set_target_properties(facetools PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/build/library/libfacetools.so"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/library/include"
)

add_executable(facegrep ${BINARY_SOURCES})
add_dependencies(facegrep facetools facegrep_library)
link_directories(${LINK_DIRECTORIES})
target_link_libraries (facegrep LINK_PUBLIC ${LINK_LIBRARIES})

install(TARGETS facegrep DESTINATION /usr/local/bin)