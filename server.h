#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#define WIN32_MEAN_AND_LEAN

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

class webServer {
private:
    bool run;
    std::string directory;
    std::string indexBuffer;

    std::mutex bufferMutex;

public:
    webServer();
    bool isRunning() { return run; }
    bool runServer(int port);
    void handleConnection(SOCKET clientSocket);
    bool startServer();
    void stopServer();
    bool setDirectory(std::string &dir);
    std::string getDirectory();
    bool setIndexBuffer(std::string &index);
    std::string getIndexBuffer(int &length);
    bool bufferIndexFile();
};

#endif // SERVER_H_INCLUDED
