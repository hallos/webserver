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
std::string TCPClientSocket::receiveData()
{
    char recBuffer[1000];
    int numBytes = recv(socket_, recBuffer, sizeof(recBuffer), 0);
    if (numBytes == -1 || numBytes == 0)
    {
        return std::string();
    }
    return std::string(recBuffer, numBytes);
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