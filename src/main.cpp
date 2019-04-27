#include "HTTP.h"
#include "Server.h"
#include <FileReader.h>
#include "WebConnectionHandler.h"
#include <vector>
#include <iostream>
#include <clara.hpp>


int main(int argc, char** argv)
{
    int port = 8080;
    std::string rootDirectory = "test/system";
    auto cli = clara::Opt(port, "port")
                ["-p"]["--port"]
                ("Port to be used by webserver to receive connections." )
             | clara::Opt(rootDirectory, "rootDirectory")
                ["-d"]["--root-directory"]
                ("Root directory of webserver. (Where to find index.html)" );
    auto result = cli.parse(clara::Args(argc, argv));
    if (!result) 
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(1);
    }

    auto serverSocket = std::make_shared<TCPServerSocket>(port);
    auto fileReader = std::make_shared<FileReader>(rootDirectory);
    auto connectionHandler = std::make_shared<WebConnectionHandler>(fileReader);
    Server webserver(serverSocket, connectionHandler, 4);
    webserver.startServer();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
