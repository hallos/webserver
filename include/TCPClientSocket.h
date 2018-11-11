#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

typedef int Socket;

#include <string>

class TCPClientSocket
{
public:
    TCPClientSocket(Socket socket);
    ~TCPClientSocket();

    std::string receiveData();
    bool sendData(const std::string& buffer);
private:
    Socket socket_;
};

#endif