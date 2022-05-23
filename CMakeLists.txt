cmake_minimum_required(VERSION 3.16)
project(SFML_project)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Request static SFML libraries when building statically
if(NOT BUILD_SHARED_LIBS)
	set(SFML_STATIC_LIBRARIES true)
endif()

add_subdirectory(src)
