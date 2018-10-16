#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include "http.h"
#include "fileReader.h"
#include "TCPClientSocket.h"

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
    void handleConnection(std::unique_ptr<TCPClientSocket> clientSocket, std::string data);
    bool startServer();
    void stopServer();

    bool setDirectory(string &dir);

};

#endif // SERVER_H_INCLUDED
