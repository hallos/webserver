#include "Server.h"
#include <vector>

#include "TCPClientSocket.h"
#include "Logger.h"


Server::Server(std::shared_ptr<ctpl::thread_pool> threadPool, 
               std::shared_ptr<TCPServerSocket> serverSocket,
               std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket)> handleConnection): 
                    threadPool_(threadPool),
                    serverSocket_(serverSocket),
                    handleConnection(handleConnection)
{
    run = false; //Set run-flag as false by default
}

bool Server::startServer()
{
    if(!run){
        runMutex.lock();
        run = true;
        runMutex.unlock();
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
    runMutex.lock();
    run = false;
    runMutex.unlock();
}

bool Server::isRunning()
{
    runMutex.lock();
    bool tmp = run;
    runMutex.unlock();
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
                std::shared_ptr<TCPClientSocket> sharedSocket = std::move(clientSocket);
                threadPool_->push(handleConnection, sharedSocket);
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