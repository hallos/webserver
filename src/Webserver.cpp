#include "Webserver.h"
#include "http.h"

#include <sstream>
#include <memory>
#include <iostream>
#include <vector>

#include "TCPServerSocket.h"
#include "TCPClientSocket.h"

using namespace std;

//--------------------------------------------
//  Constructor
//--------------------------------------------
webServer::webServer()
{
    run = false; //Set run-flag as false by default
}
//--------------------------------------------
// Deconstructor
//--------------------------------------------
webServer::~webServer()
{
    //deconstructor
}
//--------------------------------------------
// startServer
//--------------------------------------------
bool webServer::startServer()
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
//--------------------------------------------
// stopServer
// Takes an bool-pointer and sets the bool to false
// Meant to be used to stop the server from another thread
//--------------------------------------------
void webServer::stopServer()
{
    runMutex.lock();
    run = false;
    runMutex.unlock();
}
//--------------------------------------------
//  isRunning
//--------------------------------------------
bool webServer::isRunning()
{
    runMutex.lock();
    bool tmp = run;
    runMutex.unlock();
    return tmp;
}
//--------------------------------------------
// runServer
// Initialize sockets at given port and starts listening
// to incoming connections with select().
// Each new connection gets handled in a new thread
// with handleConnection
//--------------------------------------------
bool webServer::runServer(int port){
    TCPServerSocket serverSocket("0.0.0.0", port);
    std::vector<thread*> connectionThreads;

    while(this->isRunning())
    {
        std::string data;
        auto clientSocket = serverSocket.acceptConnection(data);
        if (!clientSocket)
        {
            return false;
        }    
        //clientSocket->sendData("Hello World!");
        //Open a new thread that handles the connection
        connectionThreads.push_back(
            new thread(&webServer::handleConnection,
                       this,
                       std::move(clientSocket),
                       data));
                   
    }

    //Join and delete connection-threads
    for(auto i : connectionThreads){
        i->join();
        delete i;
    }

    return true;
}
//--------------------------------------------
// handleConnection
// Takes a socket to a accepted connection
// and handles the incoming HTTP request
//--------------------------------------------
void webServer::handleConnection(std::unique_ptr<TCPClientSocket> clientSocket, std::string msg)
{
    //char recBuffer[1000];
    //recv(clientSocket, recBuffer, sizeof(recBuffer), 0);
    //cout << "Incoming req:" << endl << recBuffer << endl;
    //string receivedRequest = recBuffer;
    unique_ptr<httpInterpreter> interpreter (new httpInterpreter(msg));
    string requestedFile;
    if(interpreter->interpretRequest(requestedFile))
    {
        string content, contType;
        if(reader.getFile(requestedFile, content, contType))
        {
            interpreter->constructResponse(content, contType);
        }
        string message = interpreter->getResponse();
        // Send response
        clientSocket->sendData(message);
    }
}

bool webServer::setDirectory(string &dir)
{
    return reader.setDirectory(dir);
}
