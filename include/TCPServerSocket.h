#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#define SOCKET int
#define INVALID_SOCKET -1

#include <memory>
#include <exception>
#include "TCPClientSocket.h"

class TCPSocketException : public std::exception
{
public:
    TCPSocketException(const std::string& what): what_(what) {};
    virtual ~TCPSocketException() {};
    const std::string what() { return what_; }
private:
    std::string what_;
};

class TCPServerSocket
{
public:
    TCPServerSocket(int port);
    ~TCPServerSocket();

    std::unique_ptr<TCPClientSocket> acceptConnection();
private:
    int port_;
    SOCKET socket_;
};

#endif 