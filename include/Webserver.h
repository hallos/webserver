#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include "http.h"
#include "FileReader.h"
#include "TCPServerSocket.h"
#include "ctpl_stl.h"


class Webserver 
{
private:
    bool run;
    std::mutex runMutex;
    std::shared_ptr<ctpl::thread_pool> threadPool_;
    std::shared_ptr<FileReader> fileReader_;
    std::shared_ptr<TCPServerSocket> serverSocket_;

    void runServer(int port);

public:
    Webserver(std::shared_ptr<ctpl::thread_pool> threadPool, 
              std::shared_ptr<FileReader> fileReader,
              std::shared_ptr<TCPServerSocket> serverSocket);
    ~Webserver();

    bool isRunning();
    bool startServer();
    void stopServer();
};

#endif
