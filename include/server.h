#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#ifdef WINDOWS
    #define WIN32_MEAN_AND_LEAN
    #include <winsock2.h>
    #include <windows.h>
#endif
#include <thread>
#include <mutex>
#include <string>
#include "http.h"
#include "fileReader.h"

using namespace std;

class webServer {
private:
    bool run;
    mutex runMutex;

    fileReader reader;

public:
    webServer();
    ~webServer();

    bool isRunning();
    bool runServer(int port);
    void handleConnection(SOCKET clientSocket);
    bool startServer();
    void stopServer();

    bool setDirectory(string &dir);

};

#endif // SERVER_H_INCLUDED
