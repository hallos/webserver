#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#define INVALID_SOCKET -1
typedef int Socket;

#include <string>

class TCPClientSocket
{
public:
    TCPClientSocket(Socket socket);
    ~TCPClientSocket();

    bool sendData(const std::string& buffer);
private:
    Socket socket_;
};

#endif