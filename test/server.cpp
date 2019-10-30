#include "catch.hpp"

#include <deque>
#include <string>
#include <thread>
#include <chrono>
#include "Server.h"
#include "TCPStreamSocket.h"
#include "TCPServerSocket.h"

#include <iostream>


class MockConnectionHandler : public ConnectionHandler
{
public:
    MockConnectionHandler() {};
    ~MockConnectionHandler() {};
    void onAccept(std::shared_ptr<ITCPStreamSocket> clientSocket)
    {
        std::string request = clientSocket->receiveData();
        std::string response = "Received: " + request;
        clientSocket->sendData(response);     
    }
};

class MockTCPStreamSocket : public ITCPStreamSocket
{
public:
    MockTCPStreamSocket(std::string receivedData, std::shared_ptr<std::string> sentData):
        receivedData_(receivedData), sentData_(sentData) {}
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

class MockTCPServerSocket : public ITCPServerSocket
{ 
public:
    MockTCPServerSocket() {}
    std::unique_ptr<ITCPStreamSocket> acceptConnection()
    {
        std::unique_ptr<MockTCPStreamSocket> clientSocket;
        if (!incomingConnections.empty())
        {
            clientSocket = std::move(incomingConnections.front());
            incomingConnections.pop_front();
        }
        return clientSocket;
    }
    void setIncomingConnection(std::unique_ptr<MockTCPStreamSocket> conn)
    {
        incomingConnections.push_back(std::move(conn));
    }
private:
    std::deque<std::unique_ptr<MockTCPStreamSocket>> incomingConnections;
};

TEST_CASE("Server responds to request")
{
    auto serverSocket = std::make_shared<MockTCPServerSocket>();
    auto connHandler = std::make_shared<MockConnectionHandler>();
    Server server(serverSocket, connHandler, 1);
    server.startServer();
    // Send request
    auto receivedData = std::make_shared<std::string>();
    std::string request = "HelloServer!";
    auto clientSocket = std::make_unique<MockTCPStreamSocket>(request, receivedData);
    REQUIRE(*receivedData == "");

    serverSocket->setIncomingConnection(std::move(clientSocket));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    server.stopServer();

    std::string expectedResponse = "Received: " + request;
    REQUIRE(expectedResponse == *receivedData);
}

TEST_CASE("Server responds to multiple requests")
{
    auto serverSocket = std::make_shared<MockTCPServerSocket>();
    auto connHandler = std::make_shared<MockConnectionHandler>();
    Server server(serverSocket, connHandler, 1);
    server.startServer();

    auto receivedData = std::make_shared<std::string>();
    std::string request = " HelloServer!";

    for (int i = 35; i < 45; i++)
    {
        request[0] = i;
        serverSocket->setIncomingConnection(
            std::make_unique<MockTCPStreamSocket>(
                request,
                receivedData
            )
        );
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::string expectedResponse = "Received: " + request;
        REQUIRE(expectedResponse == *receivedData);
    }
    server.stopServer();
}