#include <thread>
#include <vector>
#include "HTTP.h"
#include "Server.h"
#include <FileReader.h>
#include "ui.h"
#include "ctpl_stl.h"
#include "WebConnectionHandler.h"


int main()
{
    system("title webServer"); //Set title of command-prompt
    std::vector<std::unique_ptr<std::thread>> Threads;
    auto threadPool = std::make_shared<ctpl::thread_pool>(4);
    // TODO: Read root dir and port from config file
    std::string rootDir = "/home/oscar/Documents/Projekt/hello_web";
    int port = 8090;
    auto serverSocket = std::make_shared<TCPServerSocket>(port);
    auto fileReader = std::make_shared<FileReader>(rootDir);
    auto connectionHandler = std::make_shared<WebConnectionHandler>(fileReader);
    Server webserver(threadPool, serverSocket, connectionHandler);
    
    bool exit = false;
    do{
        printMainMenu(webserver.isRunning());
        switch(getMenuOption(3))
        {
        case 1:
            Threads.emplace_back(new std::thread(&Server::startServer,&webserver));
            break;
        case 2:
            webserver.stopServer();
            break;
        case 0:
            webserver.stopServer();
            exit=true;
            break;
        default:
            break;
        }

    }while(!exit);
    for (auto& t : Threads)
    {
        t->join();
    }
    return 0;
}
