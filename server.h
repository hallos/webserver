#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#define WIN32_MEAN_AND_LEAN

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>

class webServer {
private:
    bool run;

public:
    bool isRunning() { return run; }
    bool runServer(int port);
    void handleConnection(SOCKET clientSocket);
    bool startServer();
    void stopServer();
};

#endif // SERVER_H_INCLUDED
