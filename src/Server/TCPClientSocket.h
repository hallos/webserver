#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#ifndef WIN32
#define INVALID_SOCKET -1
#endif

#include <string>

typedef int Socket;

class ITCPClientSocket
{
public:
    virtual ~ITCPClientSocket() {};

    virtual std::string receiveData() = 0;
    virtual bool sendData(const std::string& buffer) = 0; 
};

class TCPClientSocket : public ITCPClientSocket
{
public:
    TCPClientSocket(Socket socket);
    virtual ~TCPClientSocket();

    std::string receiveData();
    bool sendData(const std::string& buffer);
private:
    Socket socket_;
};

#endif