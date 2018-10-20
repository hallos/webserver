#include "catch.hpp"

#include <dequeue>
#include "Webserver.h"
#include "FileReader.h"
#include "TCPClientSocket.h"
#include "TCPServerSocket.h"

std::string indexHtml = "<html><body>Hello World Wide Web!</body></html>"

class MockFileReader : public FileReader
{
public:
    MockFileReader(): FileReader("") {}
    bool getFile(const std::string &filename, std::string &retFileContent, std::string &retContentType)
    {
        if (filename == "index.html")
        {
            retFileContent = indexHtml;
            retContentType = "text/html";
            return true;
        }
        return false;
    }
};

class MockTCPClientSocket : public TCPClientSocket
{
public:
    MockTCPClientSocket(std::string receivedData): TCPClientSocket(-1), receivedData_(receiveData) {}
    bool sendData(const std::string& buffer)
    {
        sentData_ = buffer;
    }
    std::string receiveData()
    {
        return receivedData_;
    }
private:
    std::string sentData_;
    std::string receivedData_;
};

class MockTCPServerSocket : public TCPServerSocket
{
public:
    MockTCPServerSocket(): TCPServerSocket(8090) {}
    std::unique_ptr<TCPClientSocket> acceptConnection()
    {
        std::unique_ptr<MockTCPClientSocket> clientSocket;
        if (!incomingConnections.empty())
        {
            clientSocket = std::move(incomingConnections.front());
            incomingConnections.pop_front();
        }
        return clientSocket;
    }
    void setIncomingConnection(std::shared_ptr<MockTCPClientSocket> conn)
    {
        incomingConnections.push_back(conn);
    }
private:
    std::dequeue<std::shared<MockTCPClientSocket>> incomingConnections;
};