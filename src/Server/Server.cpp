#include "Server.h"
#include <vector>
#include "TCPStreamSocket.h"
#include "Logger.h"


Server::Server(std::shared_ptr<ITCPServerSocket> serverSocket,
               std::shared_ptr<ConnectionHandler> connectionHandler,
               int numThreads) :
                    run_(false),
                    serverSocket_(serverSocket),
                    connectionHandler_(connectionHandler)
{
    // Start up numThreads + 1 extra to be used as server thread
    threadPool_ = std::make_shared<ctpl::thread_pool>(numThreads + 1);
}

bool Server::startServer()
{
    if (!run_)
    {
        runMutex_.lock();
        run_ = true;
        threadPool_->push(std::bind(&Server::runServer, this));
        runMutex_.unlock();
        return true;
    }
    else
    {
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
    return run_;
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