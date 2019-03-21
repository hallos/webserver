cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
project(Webserver)

enable_testing()

include_directories(external src)

add_subdirectory(src)                       
add_subdirectory(test)

install(TARGETS Webserver
        RUNTIME DESTINATION /usr/local/bin)
