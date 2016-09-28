#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#define WIN32_MEAN_AND_LEAN

#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include <string>
#include "http.h"

using namespace std;

class webServer {
private:
    bool run;
    // Mutexes for datamembers
    mutex runMutex;

public:
    webServer();
    ~webServer();

    bool isRunning();
    bool runServer(int port);
    void handleConnection(SOCKET clientSocket);
    bool startServer();
    void stopServer();

};

#endif // SERVER_H_INCLUDED
