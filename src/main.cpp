#include <vector>
#include "HTTP.h"
#include "Server.h"
#include <FileReader.h>
#include "ctpl_stl.h"
#include "WebConnectionHandler.h"


int main()
{
    auto threadPool = std::make_shared<ctpl::thread_pool>(4);
    // TODO: Read root dir and port from config file
    std::string rootDir = "/home/oscar/Documents/Projekt/hello_web";
    int port = 8090;
    auto serverSocket = std::make_shared<TCPServerSocket>(port);
    auto fileReader = std::make_shared<FileReader>(rootDir);
    auto connectionHandler = std::make_shared<WebConnectionHandler>(fileReader);
    Server webserver(threadPool, serverSocket, connectionHandler);
    
    webserver.startServer();

    return 0;
}
