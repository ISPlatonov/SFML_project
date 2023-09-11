cmake_minimum_required(VERSION 3.16)
project(SFML_project LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include)

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(SFML_project ${SOURCE_FILES})
target_compile_definitions(SFML_project PRIVATE CLIENT)
target_link_libraries(SFML_project sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
target_compile_features(SFML_project PRIVATE cxx_std_17)

set(SERVER_SOURCE_FILES src/server/server.cpp src/Multiplayer.cpp src/Object.cpp)
add_executable(server ${SERVER_SOURCE_FILES})
target_link_libraries(server sfml-graphics sfml-window sfml-network sfml-system)
target_compile_features(server PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET SFML_project
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:SFML_project>
        VERBATIM)
endif()

add_custom_command(TARGET SFML_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:SFML_project>/textures)
add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:server>/textures)

install(TARGETS SFML_project server DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_SOURCE_DIR}/bin)
