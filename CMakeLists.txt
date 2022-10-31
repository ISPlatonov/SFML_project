cmake_minimum_required(VERSION 3.16)
project(SFML_project)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS graphics window network system)
if (NOT SFML_FOUND)
    set(SFML_STATIC_LIBRARIES FALSE)
    find_package(SFML 2 REQUIRED COMPONENTS graphics window network system)
endif()

add_subdirectory(src)