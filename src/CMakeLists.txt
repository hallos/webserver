add_subdirectory(FileReader)
add_subdirectory(Server)
include_directories(
    FileReader
    Server
)

add_library(WebserverLib STATIC
    HTTP.cpp
    Logger.cpp
    WebConnectionHandler.cpp
)

target_link_libraries(WebserverLib 
    FileReader
    Server
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
