cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
if (NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local)
endif()
if (NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR bin)
endif()
project(Webserver)

enable_testing()

include_directories(
    external
    external/hallib/include
    external/hallib/src/network
    src
)

add_subdirectory(external/hallib)
add_subdirectory(src)
add_subdirectory(test)
