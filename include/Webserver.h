#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include "http.h"
#include "fileReader.h"
#include "TCPClientSocket.h"
#include "ctpl_stl.h"

using namespace std;

class Webserver {
private:
    bool run;
    mutex runMutex;
    std::shared_ptr<ctpl::thread_pool> threadPool_;
    std::shared_ptr<fileReader> reader;

public:
    Webserver(std::shared_ptr<ctpl::thread_pool> threadPool);
    ~Webserver();

    bool isRunning();
    bool runServer(int port);
    bool startServer();
    void stopServer();

    bool setDirectory(string &dir);

};

#endif
