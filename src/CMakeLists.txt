include_directories(${SFML_project_SOURCE_DIR}/include)

set(SOURCE_FILES main.cpp Multiplayer.cpp Actor.cpp WorldMap.cpp Controls.cpp Object.cpp LinearAlgebra.cpp)
set(SERVER_SOURCE_FILES server.cpp Multiplayer.cpp Object.cpp)

if (BUILD_SERVER_ONLY)
    add_executable(server ${SERVER_SOURCE_FILES})

    find_package(SFML 2 REQUIRED COMPONENTS graphics window network system)
    target_link_libraries(server sfml-graphics sfml-window sfml-network sfml-system)

    install(TARGETS server DESTINATION ${CMAKE_SOURCE_DIR}/bin)


else()
    add_executable(SFML_project ${SOURCE_FILES})
    add_executable(server ${SERVER_SOURCE_FILES})

    target_include_directories(SFML_project PUBLIC include)

    find_package(SFML 2 REQUIRED COMPONENTS graphics window audio network system)
    target_link_libraries(SFML_project sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
    target_link_libraries(server sfml-graphics sfml-window sfml-network sfml-system)

    add_custom_command(TARGET SFML_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:SFML_project>/textures)

    install(TARGETS SFML_project server DESTINATION ${CMAKE_SOURCE_DIR}/bin)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_SOURCE_DIR}/bin)

endif()
