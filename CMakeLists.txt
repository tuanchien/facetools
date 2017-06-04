#
# This is a CMake makefile.  You can find the cmake utility and
# information about it at http://www.cmake.org
#

cmake_minimum_required(VERSION 2.8)

PROJECT(facetools_suite)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory (library)
add_subdirectory (tests/facetools)

add_subdirectory (apps/facegrep)
add_subdirectory (tests/facegrep)

enable_testing ()
add_test (NAME facetools_validation COMMAND facetools_validation)
add_test (NAME facegrep_validation COMMAND facegrep_validation)