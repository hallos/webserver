#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <functional>
#include "TCPServerSocket.h"
#include "ctpl_stl.h"


class ConnectionHandler
{
public:
    ConnectionHandler() {};
    virtual ~ConnectionHandler() {};
    virtual void onAccept(int id,std::shared_ptr<ITCPStreamSocket> clientSocket) = 0;
};

class Server 
{
public:
    Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
           std::shared_ptr<ITCPServerSocket> serverSocket,
           std::shared_ptr<ConnectionHandler> connectionHandler);
    ~Server() {};

    bool isRunning();
    bool startServer();
    void stopServer();
private:
    bool run_;
    std::mutex runMutex_;
    std::shared_ptr<ctpl::thread_pool> threadPool_;
    std::shared_ptr<ITCPServerSocket> serverSocket_;
    std::shared_ptr<ConnectionHandler> connectionHandler_;

    void runServer();
};

#endif
