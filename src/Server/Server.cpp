#include "Server.h"
#include <vector>
#include "TCPStreamSocket.h"
#include "Logger.h"


Server::Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
               std::shared_ptr<ITCPServerSocket> serverSocket,
               std::shared_ptr<ConnectionHandler> connectionHandler): 
                    run_(false),
                    threadPool_(threadPool),
                    serverSocket_(serverSocket),
                    connectionHandler_(connectionHandler)                
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
        auto handler = std::bind(
                &ConnectionHandler::onAccept, 
                connectionHandler_.get(), 
                std::placeholders::_1,
                std::placeholders::_2);

        while(this->isRunning())
        {
            auto clientSocket = serverSocket_->acceptConnection();
            if (clientSocket)
            {    
                std::shared_ptr<ITCPStreamSocket> sharedSocket = std::move(clientSocket);
                threadPool_->push(handler, sharedSocket);
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