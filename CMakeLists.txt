cmake_minimum_required(VERSION 3.16)
project(SFML_project LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include)
add_subdirectory(src)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_SOURCE_DIR}/bin)
