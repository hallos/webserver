cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
project(Webserver)

include_directories(external
                    include)

add_executable(Webserver main.cpp
                         src/TCPServerSocket.cpp
                         src/TCPClientSocket.cpp
                         src/http.cpp
                         src/Webserver.cpp
                         src/ui.cpp
                         src/file.cpp
                         src/fileReader.cpp)

target_link_libraries(Webserver pthread)
                        