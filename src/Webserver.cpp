#include "Webserver.h"
#include "http.h"

#include <sstream>
#include <iostream>
#include <vector>

#include "TCPServerSocket.h"
#include "TCPClientSocket.h"


Webserver::Webserver(std::shared_ptr<ctpl::thread_pool> threadPool, 
                     const std::string& rootDirectory): 
                        threadPool_(threadPool)
{
    run = false; //Set run-flag as false by default
    reader = std::make_shared<fileReader>();
    reader->setDirectory(rootDirectory);
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

void Webserver::runServer(int port){
    TCPServerSocket serverSocket(port);

    while(this->isRunning())
    {
        auto clientSocket = serverSocket.acceptConnection();
        if (clientSocket)
        {    
            threadPool_->push(
                [clientSocket = std::move(clientSocket), reader = reader](int id)
                {
                    std::string msg = clientSocket->receiveData();
                    httpInterpreter interpreter(msg);
                    std::string requestedFile;
                    if (interpreter.interpretRequest(requestedFile))
                    {
                        std::string content, contType;
                        if (reader->getFile(requestedFile, content, contType))
                        {
                            interpreter.constructResponse(content, contType);
                        }
                        std::string response = interpreter.getResponse();
                        // Send response
                        clientSocket->sendData(response);
                    }
                });
        }          
    }
}
