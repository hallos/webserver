#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <functional>
#include "TCPServerSocket.h"
#include "ctpl_stl.h"


class Server 
{
private:
    bool run;
    std::mutex runMutex;
    std::shared_ptr<ctpl::thread_pool> threadPool_;
    std::shared_ptr<TCPServerSocket> serverSocket_;
    std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket)> handleConnection;

    void runServer();
    
public:
    Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
           std::shared_ptr<TCPServerSocket> serverSocket,
           std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket)> handleConnection);
    ~Server() {};

    bool isRunning();
    bool startServer();
    void stopServer();
};

#endif
