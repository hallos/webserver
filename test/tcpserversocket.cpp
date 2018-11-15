#define CATCH_CONFIG_MAIN 
#include "catch.hpp"

#include "TCPServerSocket.h"
#include <iostream>

TEST_CASE( "", "[serversocket]" ) 
{
    TCPServerSocket serverSocket(8080); 
    std::string data = "Hello Socket!";
    auto clientSocket = serverSocket.acceptConnection();

    REQUIRE(clientSocket);

    std::cout << "Received: " << data;
}