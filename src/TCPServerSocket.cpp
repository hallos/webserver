#include "TCPServerSocket.h"

#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h> //close

#include "Logger.h"

/**
 * Constructor
 */ 
TCPServerSocket::TCPServerSocket(int port)
{
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET)
    {
        Logger::log("TCPServerSocket: Could not create socket.");
        throw TCPSocketException("Could not create socket.");
    } 

    sockaddr_in sockAdr = {0};
    sockAdr.sin_family = AF_INET;
    sockAdr.sin_port = htons(port);  
    sockAdr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_, reinterpret_cast<sockaddr*>(&sockAdr), sizeof(sockAdr)) != 0)
    {
        Logger::log("TCPServerSocket: Couldn't bind socket. Error code: " + errno);
        throw TCPSocketException("Could not bind socket.");
    }

    if (listen(socket_, SOMAXCONN)!=0)
    {
        Logger::log("TCPServerSocket: Couldn't set socket in listening mode. Error code: " + errno);
        throw TCPSocketException("Could not set socket in listening mode.");
    }   
}

/**
 * Destructor
 */ 
TCPServerSocket::~TCPServerSocket()
{
    if (socket_ != INVALID_SOCKET)
    {
        close(socket_);
    }
}

/**
 * 
 */
std::unique_ptr<TCPClientSocket> TCPServerSocket::acceptConnection()
{
    sockaddr_in clientSockAdr;
    socklen_t clientSockSize = sizeof(clientSockAdr);

    /*fd_set fdRead;
    FD_ZERO(&fdRead);
    FD_SET(socket_,&fdRead);
    struct timeval timeOutTime;// = {timeout,0};

    timeOutTime.tv_sec = timeout;
    timeOutTime.tv_usec = 0;

    int retVal = select(1,&fdRead,NULL,NULL,&timeOutTime);
    if (retVal <= 0)
    {
        return std::unique_ptr<TCPClientSocket>();
    }

    if (!FD_ISSET(socket_,&fdRead))
    {
        return std::unique_ptr<TCPClientSocket>();
    }*/

    SOCKET clientSocket = accept(socket_, reinterpret_cast<sockaddr*>(&clientSockAdr),&clientSockSize);
    if (clientSocket == INVALID_SOCKET)
    {
        return std::unique_ptr<TCPClientSocket>();
    }

    return std::make_unique<TCPClientSocket>(clientSocket);    
} 