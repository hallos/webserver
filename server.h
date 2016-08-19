#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#define WIN32_MEAN_AND_LEAN

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <string>

class webServer {
private:
    bool run;
    std::string directory;
    std::string indexFile;

public:
    bool isRunning() { return run; }
    bool runServer(int port);
    void handleConnection(SOCKET clientSocket);
    bool startServer();
    void stopServer();
    bool setDirectory(std::string dir);
    bool bufferIndexFile();
};

#endif // SERVER_H_INCLUDED
