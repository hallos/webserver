project(WebserverTest)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test/tmp)

include_directories(
    ../src
    ../src/FileReader
    ../src/Server
)

add_executable(WebserverTest 
    main.cpp
    http.cpp
    filereader.cpp
    server.cpp
)

target_link_libraries(WebserverTest 
    WebserverLib
    Server
    pthread
)

add_test(
    Webserver
    WebserverTest
)