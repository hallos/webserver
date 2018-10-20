#include <thread>
#include <vector>

#include "Webserver.h"
#include "ui.h"
#include "ctpl_stl.h"


int main()
{
    system("title webServer"); //Set title of command-prompt
    std::vector<std::unique_ptr<thread>> Threads;
    auto threadPool = std::make_shared<ctpl::thread_pool>(4);
    // TODO: Read root dir from config file
    std::string rootDir = "/home/oscar/Documents/Projekt/hello_web";
    auto fileReader = std::make_shared<FileReader>(rootDir);
    Webserver server(threadPool, fileReader);
    bool exit = false;

    do{
        printMainMenu(server.isRunning());
        switch(getMenuOption(3))
        {
        case 1:
            Threads.emplace_back(new thread(&Webserver::startServer,&server));
            break;
        case 2:
            server.stopServer();
            break;
        case 0:
            server.stopServer();
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
