cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
project(Webserver)

add_library(WebserverLib SHARED
                         src/TCPServerSocket.cpp
                         src/TCPClientSocket.cpp
                         src/http.cpp
                         src/Webserver.cpp
                         src/ui.cpp
                         src/File.cpp
                         src/FileReader.cpp
                         src/Logger.cpp)

include_directories(external
                    include)

add_executable(Webserver main.cpp)

target_link_libraries(Webserver pthread 
                                WebserverLib)

add_subdirectory(test)                       