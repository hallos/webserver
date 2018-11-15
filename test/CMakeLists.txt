cmake_minimum_required(VERSION 2.8.9)

project(WebserverTest)

add_executable(WebserverTest main.cpp
                             http.cpp)

target_link_libraries(WebserverTest WebserverLib
                                    pthread)