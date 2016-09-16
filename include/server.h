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

using namespace std;

class webServer {
private:
    bool run;
    string directory;
    string indexBuffer;
    // Mutexes for datamembers
    mutex bufferMutex;
    mutex runMutex;
    mutex dirMutex;

public:
    webServer();
    ~webServer();

    bool isRunning();
    bool runServer(int port);
    void handleConnection(SOCKET clientSocket);
    bool startServer();
    void stopServer();
    bool setDirectory(string &dir);
    string getDirectory();
    bool setIndexBuffer(string &index);
    string getIndexBuffer(int &length);
    bool bufferIndexFile();
};

#endif // SERVER_H_INCLUDED
