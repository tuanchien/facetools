cmake_minimum_required(VERSION 2.8)

PROJECT(facetools_library)

set (CMAKE_CXX_FLAGS "--std=c++14 -O3")

file(GLOB_RECURSE SOURCES src/*.cpp)

set(INCLUDES
${CMAKE_SOURCE_DIR}/library/include
)

include_directories(${INCLUDES})

add_library(facetools_static STATIC ${SOURCES})
add_library(facetools_shared SHARED ${SOURCES})

set_target_properties(facetools_static PROPERTIES OUTPUT_NAME facetools)
set_target_properties(facetools_shared PROPERTIES OUTPUT_NAME facetools)

add_subdirectory (tests)
