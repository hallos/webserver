
include_directories(../)
add_library(Sockets STATIC
    TCPServerSocket.cpp
    TCPClientSocket.cpp
)