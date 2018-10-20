#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include "http.h"
#include "FileReader.h"
#include "TCPClientSocket.h"
#include "ctpl_stl.h"


class Webserver 
{
private:
    bool run;
    std::mutex runMutex;
    std::shared_ptr<ctpl::thread_pool> threadPool_;
    std::shared_ptr<FileReader> fileReader_;

    void runServer(int port);

public:
    Webserver(std::shared_ptr<ctpl::thread_pool> threadPool, const std::string& rootDirectory);
    ~Webserver();

    bool isRunning();
    bool startServer();
    void stopServer();
};

#endif
