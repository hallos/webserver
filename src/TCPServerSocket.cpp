#include "TCPServerSocket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> //close

/**
 * Constructor
 */ 
TCPServerSocket::TCPServerSocket(std::string hostname, int port)
{
    socket_ = INVALID_SOCKET;
    sockaddr_in sockAdr = {0};

    if((socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
        //std::cerr << "INVALID Socket.";
    }

    //Set sockAddr
    sockAdr.sin_family = AF_INET;
    sockAdr.sin_port = htons(port);  
    sockAdr.sin_addr.s_addr = htonl(INADDR_ANY);

    //Bind socket
    if(bind(socket_,reinterpret_cast<sockaddr*>(&sockAdr),sizeof(sockAdr)) != 0){
        //std::cerr << "Couldn't bind socket. Error code: " << errno;
    }

    //Set socket in listening mode
    if(listen(socket_, SOMAXCONN)!=0){
        //std::cerr << "Couldn't set socket in listening-mode. Error code: " << errno;
    }        
}

/**
 * Destructor
 */ 
TCPServerSocket::~TCPServerSocket()
{
    //Close server-socket
    if(socket_ != INVALID_SOCKET){
        close(socket_);
    }
}

/**
 * 
 */
std::unique_ptr<TCPClientSocket> TCPServerSocket::acceptConnection(std::string& message)
{
    sockaddr_in clientSockAdr;
    socklen_t clientSockSize = sizeof(clientSockAdr);

    SOCKET clientSocket = accept(socket_, reinterpret_cast<sockaddr*>(&clientSockAdr),&clientSockSize);
    if (clientSocket==INVALID_SOCKET)
    {
        //cerr << "Accept connection failed. Error code: " << errno;
        return std::unique_ptr<TCPClientSocket>();
    }
    char recBuffer[1000];
    recv(clientSocket, recBuffer, sizeof(recBuffer), 0);
    message = recBuffer;
    return std::unique_ptr<TCPClientSocket>(new TCPClientSocket(clientSocket));//std::make_unique<TCPClientSocket>(clientSocket);       
} 