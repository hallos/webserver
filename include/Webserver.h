#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include "http.h"
#include "fileReader.h"
#include "TCPClientSocket.h"

using namespace std;

class Webserver {
private:
    bool run;
    mutex runMutex;

    fileReader reader;

public:
    Webserver();
    ~Webserver();

    bool isRunning();
    bool runServer(int port);
    void handleRequest(std::unique_ptr<TCPClientSocket> clientSocket);
    bool startServer();
    void stopServer();

    bool setDirectory(string &dir);

};

#endif
