#include "Webserver.h"
#include <vector>

#include "http.h"
#include "TCPClientSocket.h"
#include "Logger.h"


Webserver::Webserver(std::shared_ptr<ctpl::thread_pool> threadPool, 
                     std::shared_ptr<FileReader> fileReader,
                     std::shared_ptr<TCPServerSocket> serverSocket,
                     std::function<void(int id, std::shared_ptr<TCPClientSocket> clientSocket, std::shared_ptr<FileReader> fileReader)> handleConnection): 
                        threadPool_(threadPool),
                        fileReader_(fileReader),
                        serverSocket_(serverSocket),
                        handleConnection(handleConnection)
{
    run = false; //Set run-flag as false by default
}

Webserver::~Webserver()
{
    //destructor
}

bool Webserver::startServer()
{
    if(!run){
        runMutex.lock();
        run = true;
        runMutex.unlock();
        runServer(8090);
        return true;
    }
    else{
        Logger::log("Webserver::startServer(): Server is already running.");
        return false;
    }
}

void Webserver::stopServer()
{
    runMutex.lock();
    run = false;
    runMutex.unlock();
}

bool Webserver::isRunning()
{
    runMutex.lock();
    bool tmp = run;
    runMutex.unlock();
    return tmp;
}

void Webserver::runServer(int port){
    try
    {
        while(this->isRunning())
        {
            auto clientSocket = serverSocket_->acceptConnection();
            if (clientSocket)
            {    
                std::shared_ptr<TCPClientSocket> sharedSocket = std::move(clientSocket);
                threadPool_->push(handleConnection, sharedSocket, fileReader_);
            }          
        }
    }
    catch (TCPSocketException& e)
    {
        Logger::log("Webserver::runServer(): Socket failed: " + e.what());
        Logger::log("Webserver::runServer(): Shutting down server");
        stopServer();
        return;
    }
}
