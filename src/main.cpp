#include <thread>
#include <vector>
#include "HTTP.h"
#include "Server.h"
#include <FileReader.h>
#include "ui.h"
#include "ctpl_stl.h"


int main()
{
    system("title webServer"); //Set title of command-prompt
    std::vector<std::unique_ptr<std::thread>> Threads;
    auto threadPool = std::make_shared<ctpl::thread_pool>(4);
    // Define function for handling incoming connections
    auto handleConnection = [](int id, std::shared_ptr<ITCPStreamSocket> clientSocket, std::any sharedObject)
        {
            std::string request = clientSocket->receiveData();
            std::string requestedFile = HTTP::interpretGETRequest(request);

            auto fileReader = std::any_cast<std::shared_ptr<FileReader>>(sharedObject);
            auto file = fileReader->getFile(requestedFile);
            std::string response;
            if (file)
            {
                response = HTTP::constructOKResponse(file->getContent(), file->getContentType());
            }
            else
            {
                response = HTTP::constructNotFoundResponse(requestedFile);
            }
            clientSocket->sendData(response);
        };
        
    // TODO: Read root dir and port from config file
    std::string rootDir = "/home/oscar/Documents/Projekt/hello_web";
    int port = 8090;
    auto serverSocket = std::make_shared<TCPServerSocket>(port);
    std::any fileReader = std::make_shared<FileReader>(rootDir);
    Server webserver(threadPool, serverSocket, handleConnection, fileReader);
    
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
