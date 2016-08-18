#include <thread>
#include <vector>

#include "server.h"
#include "ui.h"

using namespace std;

int main()
{
    vector<thread*> Threads;
    webServer server;
    bool exit=false;

    do{
        printMainMenu(server.isRunning());
        switch(getMenuOption(3))
        {
        case 1:
            Threads.push_back(new thread(&webServer::startServer,&server));
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
    for(auto i : Threads){
        i->join();
        delete i;
    }

    return 0;
}
