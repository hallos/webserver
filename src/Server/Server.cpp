#include "Server.h"
#include <vector>
#include "TCPClientSocket.h"
#include "Logger.h"


Server::Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
               std::shared_ptr<ITCPServerSocket> serverSocket,
               std::function<void(int id, std::shared_ptr<ITCPClientSocket> clientSocket, std::any sharedObject)> handleConnection,
               std::any sharedObject): 
                    run_(false),
                    threadPool_(threadPool),
                    serverSocket_(serverSocket),
                    handleConnection_(handleConnection),
                    sharedObject_(sharedObject)                  
{}

bool Server::startServer()
{
    if(!run_){
        runMutex_.lock();
        run_ = true;
        runMutex_.unlock();
        runServer();
        return true;
    }
    else{
        Logger::log("Server::startServer(): Server is already running.");
        return false;
    }
}

void Server::stopServer()
{
    runMutex_.lock();
    run_ = false;
    runMutex_.unlock();
}

bool Server::isRunning()
{
    runMutex_.lock();
    bool tmp = run_;
    runMutex_.unlock();
    return tmp;
}

void Server::runServer(){
    try
    {
        while(this->isRunning())
        {
            auto clientSocket = serverSocket_->acceptConnection();
            if (clientSocket)
            {    
                std::shared_ptr<ITCPClientSocket> sharedSocket = std::move(clientSocket);
                threadPool_->push(handleConnection_, sharedSocket, sharedObject_);
            }          
        }
    }
    catch (TCPSocketException& e)
    {
        Logger::log("Server::runServer(): Socket failed: " + e.what());
        Logger::log("Server::runServer(): Shutting down server");
        stopServer();
        return;
    }
}