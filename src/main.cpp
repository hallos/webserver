#include <thread>
#include <vector>
#include "http.h"
#include "Server.h"
#include <FileReader.h>
#include "ui.h"
#include "ctpl_stl.h"


int main()
{
    system("title webServer"); //Set title of command-prompt
    std::vector<std::unique_ptr<thread>> Threads;
    auto threadPool = std::make_shared<ctpl::thread_pool>(4);
    // TODO: Read root dir and port from config file
    auto handleConnection = [](int id, std::shared_ptr<TCPClientSocket> clientSocket)
        {
            std::string request = clientSocket->receiveData();
            httpInterpreter interpreter(request);
            std::string requestedFile;
            if (interpreter.interpretRequest(requestedFile))
            {
                FileReader fileReader("/home/oscar/Documents/Projekt/hello_web");
                auto file = fileReader.getFile(requestedFile);
                if (file)
                {
                    interpreter.constructResponse(file->getContent(), file->getContentType());
                }
                std::string response = interpreter.getResponse();
                // Send response
                clientSocket->sendData(response);
            }
        };
    std::string rootDir = "/home/oscar/Documents/Projekt/hello_web";
    int port = 8090;
    auto serverSocket = std::make_shared<TCPServerSocket>(port);
    Server webserver(threadPool, serverSocket, handleConnection);
    bool exit = false;

    do{
        printMainMenu(webserver.isRunning());
        switch(getMenuOption(3))
        {
        case 1:
            Threads.emplace_back(new thread(&Server::startServer,&webserver));
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
