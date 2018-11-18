
include_directories(../)
add_library(Server STATIC
    Server.cpp
    TCPServerSocket.cpp
    TCPClientSocket.cpp
)