#include "TCPClientSocket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> //close

/**
 * Constructor
 */ 
TCPClientSocket::TCPClientSocket(Socket socket): socket_(socket) {}

/**
 * Destructor
 */ 
TCPClientSocket::~TCPClientSocket()
{
    if(socket_ != INVALID_SOCKET){
        close(socket_);
    }
}

/**
 * 
 */ 
bool TCPClientSocket::sendData(const std::string& buffer)
{
    if (send(socket_, buffer.data(), buffer.size(), 0) == -1)
    {   
        //TODO: Log error
        return false;
    }
    return true;
}