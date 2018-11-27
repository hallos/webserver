cmake_minimum_required(VERSION 2.8.9)

project(WebserverTest)

include_directories(
    ../src
    ../src/Server
)

add_executable(WebserverTest 
    main.cpp
    http.cpp
    server.cpp
)

target_link_libraries(WebserverTest 
    WebserverLib
    Server
    pthread
)