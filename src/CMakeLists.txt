include_directories(${SFML_project_SOURCE_DIR}/include)
set(SOURCE_FILES main.cpp Actor.cpp WorldMap.cpp Controls.cpp LinearAlgebra.cpp)

if(WIN32)
    add_executable(SFML_project WIN32 ${SOURCE_FILES})
else()
    add_executable(SFML_project ${SOURCE_FILES})
endif()

set(SFML_STATIC_LIBRARIES TRUE)
target_include_directories(SFML_project PUBLIC include)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS graphics window audio network system)
target_link_libraries(SFML_project sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
#target_link_libraries(SFML_project -static)

add_custom_command(TARGET SFML_project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:SFML_project>/textures)

install(TARGETS SFML_project DESTINATION ${CMAKE_BINARY_DIR}/build)



