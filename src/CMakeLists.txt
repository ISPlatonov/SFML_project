add_subdirectory(common)

# Server build
set(SERVER_SOURCE_FILES server/server.cpp ${SRC_COMMON_SERVER})
add_executable(server ${SERVER_SOURCE_FILES})
target_link_libraries(server sfml-graphics sfml-window sfml-network sfml-system)
target_compile_features(server PRIVATE cxx_std_17)

add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:server>/textures)

# Client build
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS client/main.cpp ${SRC_COMMON_CLIENT})
add_executable(SFML_project ${SOURCE_FILES})
target_compile_definitions(SFML_project PRIVATE CLIENT)
target_link_libraries(SFML_project sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
target_compile_features(SFML_project PRIVATE cxx_std_17)

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

# Installing instructions
install(TARGETS SFML_project server DESTINATION ${CMAKE_SOURCE_DIR}/bin)