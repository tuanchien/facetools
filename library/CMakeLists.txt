set (CMAKE_CXX_FLAGS "--std=c++14 -O3")

FIND_PACKAGE(dlib 19.4 REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(INCLUDES
${CMAKE_SOURCE_DIR}/library/include
)

include_directories(${INCLUDES})

add_library(facetools_static STATIC ${SOURCES})
add_library(facetools_shared SHARED ${SOURCES})

set_target_properties(facetools_static PROPERTIES OUTPUT_NAME facetools)
set_target_properties(facetools_shared PROPERTIES OUTPUT_NAME facetools)

install(TARGETS facetools_shared DESTINATION /usr/local/lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION /usr/local)
install(CODE "execute_process(COMMAND chmod +x /usr/local/lib/libfacetools.so)")