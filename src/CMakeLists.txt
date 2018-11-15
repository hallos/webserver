add_subdirectory(FileReader)

include_directories(
    FileReader
    Sockets
)

add_library(WebserverLib STATIC
    TCPServerSocket.cpp
    TCPClientSocket.cpp
    http.cpp
    Webserver.cpp
    ui.cpp
    Logger.cpp
)

target_link_libraries(WebserverLib 
    FileReader
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
