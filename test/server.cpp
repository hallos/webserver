#include "catch.hpp"

#include <deque>
#include <string>
#include <thread>
#include "Server.h"
#include "TCPClientSocket.h"
#include "TCPServerSocket.h"

#include "ctpl_stl.h"
#include <iostream>


auto handleConnection = [](int id, std::shared_ptr<TCPClientSocket> clientSocket, std::any sharedObject)
    {
        std::string request = clientSocket->receiveData();
        std::string response = "Received: " + request;
        clientSocket->sendData(response);    
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

TEST_CASE("Server responds request")
{
    auto serverSocket = std::make_shared<MockTCPServerSocket>();
    auto threadPool = std::make_shared<ctpl::thread_pool>(1);
    Server server(threadPool, serverSocket, handleConnection, std::any());
    std::thread serverThread(&Server::startServer,&server);
    // Send request
    auto receivedData = std::make_shared<std::string>();
    std::string request = "HelloServer!";
    auto clientSocket = std::make_unique<MockTCPClientSocket>(request, receivedData);
    serverSocket->setIncomingConnection(std::move(clientSocket));

    server.stopServer();
    serverThread.join();

    std::string expectedResponse = "Received: " + request;
    REQUIRE(expectedResponse == *receivedData);
}