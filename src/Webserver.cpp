#include "Webserver.h"
#include <vector>

#include "http.h"
#include "TCPServerSocket.h"
#include "TCPClientSocket.h"
#include "Logger.h"


Webserver::Webserver(std::shared_ptr<ctpl::thread_pool> threadPool, 
                     std::shared_ptr<FileReader> fileReader): 
                        threadPool_(threadPool),
                        fileReader_(fileReader)
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
        TCPServerSocket serverSocket(port);

        while(this->isRunning())
        {
            auto clientSocket = serverSocket.acceptConnection();
            if (clientSocket)
            {    
                threadPool_->push(
                    [clientSocket = std::move(clientSocket), fileReader = fileReader_](int id)
                    {
                        std::string msg = clientSocket->receiveData();
                        httpInterpreter interpreter(msg);
                        std::string requestedFile;
                        if (interpreter.interpretRequest(requestedFile))
                        {
                            std::string content, contType;
                            if (fileReader->getFile(requestedFile, content, contType))
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
    catch (TCPSocketException& e)
    {
        Logger::log("Webserver::runServer(): Socket failed: " + e.what());
        Logger::log("Webserver::runServer(): Shuttind down server");
        stopServer();
        return;
    }
}
