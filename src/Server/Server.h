#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <functional>
#include <any>
#include "TCPServerSocket.h"
#include "ctpl_stl.h"


class Server 
{
private:
    bool run;
    std::mutex runMutex;
    std::shared_ptr<ctpl::thread_pool> threadPool_;
    std::shared_ptr<TCPServerSocket> serverSocket_;
    std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket, std::any sharedObject)> handleConnection_;
    std::any sharedObject_;

    void runServer();
    
public:
    Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
           std::shared_ptr<TCPServerSocket> serverSocket,
           std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket, std::any sharedObject)> handleConnection);
    Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
           std::shared_ptr<TCPServerSocket> serverSocket,
           std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket, std::any sharedObject)> handleConnection,
           std::any sharedObject);
    ~Server() {};

    bool isRunning();
    bool startServer();
    void stopServer();
};

#endif
