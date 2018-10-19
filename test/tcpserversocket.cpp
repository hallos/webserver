#define CATCH_CONFIG_MAIN 
#include "catch.hpp"

#include "TCPServerSocket.h"
#include <iostream>

TEST_CASE( "", "[serversocket]" ) 
{
    TCPServerSocket serverSocket("127.0.0.1", 8080); 
    std::string data;
    auto clientSocket = serverSocket.acceptConnection(data);

    REQUIRE(clientSocket);

    std::cout << "Received: " << data;
}