cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR /usr/local/bin)
endif()
project(Webserver)

enable_testing()

include_directories(external src)

add_subdirectory(src)
add_subdirectory(test)
