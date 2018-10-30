#include "catch.hpp"

#include <deque>
#include <string>
#include <thread>
#include "Webserver.h"
#include "FileReader.h"
#include "TCPClientSocket.h"
#include "TCPServerSocket.h"

#include "ctpl_stl.h"

#include <iostream>

const std::string indexHtml = "<html><body>Hello World Wide Web!</body></html>";
std::string httpRequest = "GET / HTTP/1.1\nHost: localhost:8090\nConnection: keep-alive\nCache-Control: max-age=0\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\nAccept-Encoding: gzip, deflate, br\nAccept-Language: sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7,nb;q=0.6";

class MockFileReader : public FileReader
{  // getFile not virtual
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
{ // sendData not virtual
public:
    MockTCPClientSocket(std::string receivedData, std::shared_ptr<std::string> sentData):
        TCPClientSocket(-1), receivedData_(receivedData), sentData_(sentData) {}
    bool sendData(const std::string& buffer)
    {
        *sentData_ = buffer;
    }
    std::string receiveData()
    {
        return receivedData_;
    }
private:
    std::shared_ptr<std::string> sentData_;
    std::string receivedData_;
};

class MockTCPServerSocket : public TCPServerSocket
{ // acceptConnection not virtual
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
    void setIncomingConnection(std::unique_ptr<MockTCPClientSocket> conn)
    {
        incomingConnections.push_back(std::move(conn));
    }
private:
    std::deque<std::unique_ptr<MockTCPClientSocket>> incomingConnections;
};

TEST_CASE("Webserver responds with requested")
{
    // Set up webserver
    auto fileReader = std::make_shared<MockFileReader>();
    auto serverSocket = std::make_shared<MockTCPServerSocket>();
    auto threadPool = std::make_shared<ctpl::thread_pool>(1);
    Webserver webserver(threadPool, fileReader, serverSocket);
    std::thread serverThread(&Webserver::startServer,&webserver);
    // Send request
    auto sentData = std::make_shared<std::string>();
    auto clientSocket = std::make_unique<MockTCPClientSocket>(httpRequest, sentData);
    serverSocket->setIncomingConnection(std::move(clientSocket));

    //webserver.stopServer();
    //serverThread.join();

    std::cout << *sentData;
    //std::size_t findResult = sentData->find(indexHtml);
    //REQUIRE(findResult != std::string::npos);
}