#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#define SOCKET int
#define INVALID_SOCKET -1

#include <string>
#include <memory>

#include "TCPClientSocket.h"

class TCPServerSocket
{
public:
    TCPServerSocket(std::string hostname, int port);
    ~TCPServerSocket();

    std::unique_ptr<TCPClientSocket> acceptConnection();
private:
    SOCKET socket_;
};

#endif 