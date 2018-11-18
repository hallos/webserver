add_subdirectory(FileReader)
add_subdirectory(Server)
include_directories(
    FileReader
    Server
)

add_library(WebserverLib STATIC
    http.cpp
    ui.cpp
    Logger.cpp
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
