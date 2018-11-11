#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#include <memory>
#include <exception>
#include "TCPClientSocket.h"

typedef int Socket;

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
    Socket socket_;
};

#endif 