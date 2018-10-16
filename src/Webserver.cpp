#include "Webserver.h"
#include "http.h"

#include <sstream>
#include <memory>
#include <iostream>
#include <vector>

#include "TCPServerSocket.h"
#include "TCPClientSocket.h"


Webserver::Webserver()
{
    run = false; //Set run-flag as false by default
}

Webserver::~Webserver()
{
    //deconstructor
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
        cout << "Server already running.." << endl;
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

bool Webserver::runServer(int port){
    TCPServerSocket serverSocket("0.0.0.0", port);
    std::vector<std::unique_ptr<std::thread>> connectionThreads;
    int timeout = 1;

    while(this->isRunning())
    {
        std::string data;
        auto clientSocket = serverSocket.acceptConnection(timeout, data);
        if (clientSocket)
        {    
            //Open a new thread that handles the connection
            connectionThreads.emplace_back(
                new thread(&Webserver::handleRequest,
                        this,
                        std::move(clientSocket),
                        data));
        }          
    }

    //Join connection-threads
    for(auto& i : connectionThreads)
    {
        i->join();
    }

    return true;
}

void Webserver::handleRequest(std::unique_ptr<TCPClientSocket> clientSocket, std::string msg)
{
    std::unique_ptr<httpInterpreter> interpreter (new httpInterpreter(msg));
    std::string requestedFile;
    if(interpreter->interpretRequest(requestedFile))
    {
        std::string content, contType;
        if(reader.getFile(requestedFile, content, contType))
        {
            interpreter->constructResponse(content, contType);
        }
        std::string response = interpreter->getResponse();
        // Send response
        clientSocket->sendData(response);
    }
}

bool Webserver::setDirectory(string &dir)
{
    return reader.setDirectory(dir);
}
