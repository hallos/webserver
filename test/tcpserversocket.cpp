#define CATCH_CONFIG_MAIN 
#include "catch.hpp"

#include "tcp_server_socket.h"
#include <iostream>

TEST_CASE( "", "[serversocket]" ) 
{
    hallos::tcp_server_socket serverSocket(8080); 
    std::string data = "Hello Socket!";
    auto clientSocket = serverSocket.acceptConnection();

    REQUIRE(clientSocket);

    std::cout << "Received: " << data;
}