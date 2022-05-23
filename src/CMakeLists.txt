include_directories(${SFML_project_SOURCE_DIR}/include)

set(SOURCE_FILES main.cpp Actor.cpp WorldMap.cpp Controls.cpp LinearAlgebra.cpp Multiplayer.cpp)
set(SERVER_SOURCE_FILES server.cpp Multiplayer.cpp)

if (BUILD_ONLY_SERVER)
    if(WIN32)
        add_executable(server WIN32 ${SERVER_SOURCE_FILES})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
        set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    else()
        add_executable(server ${SERVER_SOURCE_FILES})
    endif()

    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")

    find_package(SFML 2 REQUIRED COMPONENTS network system)
    target_link_libraries(server sfml-network sfml-system)

    install(TARGETS server DESTINATION ${CMAKE_BINARY_DIR}/build)


else()
    if(WIN32)
        add_executable(SFML_project WIN32 ${SOURCE_FILES})
        add_executable(server WIN32 ${SERVER_SOURCE_FILES})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
        set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    else()
        add_executable(SFML_project ${SOURCE_FILES})
        add_executable(server ${SERVER_SOURCE_FILES})
    endif()

    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")

    target_include_directories(SFML_project PUBLIC include)

    find_package(SFML 2 REQUIRED COMPONENTS graphics window audio network system)
    target_link_libraries(SFML_project sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
    target_link_libraries(server sfml-network sfml-system)
    #target_link_libraries(SFML_project -static)

    add_custom_command(TARGET SFML_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:SFML_project>/textures)

    install(TARGETS SFML_project DESTINATION ${CMAKE_BINARY_DIR}/build)
    install(TARGETS server DESTINATION ${CMAKE_BINARY_DIR}/build)

endif()
