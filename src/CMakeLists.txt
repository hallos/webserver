add_subdirectory(FileReader)

include_directories(
    FileReader
)

add_library(WebserverLib STATIC
    HTTP.cpp
    Logger.cpp
    WebConnectionHandler.cpp
)

target_link_libraries(WebserverLib 
    FileReader
    hallos_server
)

if (WIN32)
target_link_libraries(WebserverLib ws2_32)
endif (WIN32)

add_executable(Webserver 
    main.cpp
)

target_link_libraries(Webserver 
    pthread 
    WebserverLib
)

install(TARGETS Webserver
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)
