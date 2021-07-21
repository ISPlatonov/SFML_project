cmake_minimum_required(VERSION 3.6)
project(SFML_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set(SOURCE_FILES main.cpp Actor.cpp)
add_executable(SFML_project ${SOURCE_FILES})

set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS graphics window audio network system)
target_link_libraries(SFML_project sfml-graphics sfml-window sfml-audio sfml-network sfml-system)

install(TARGETS SFML_project DESTINATION ${CMAKE_BINARY_DIR})
