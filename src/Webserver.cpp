#include "server.h"
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


    while(this->isRunning())
    {
        std::string data;
        auto clientSocket = serverSocket.acceptConnection(data);
        if (!clientSocket)
        {
            return false;
        }    
        clientSocket->sendData("Hello World!");
    }

    return true;
}
//--------------------------------------------
// handleConnection
// Takes a socket to a accepted connection
// and handles the incoming HTTP request
//--------------------------------------------
void webServer::handleConnection(SOCKET clientSocket)
{
    char recBuffer[1000];
    recv(clientSocket, recBuffer, sizeof(recBuffer), 0);
    cout << "Incoming req:" << endl << recBuffer << endl;
    string receivedRequest = recBuffer;
    unique_ptr<httpInterpreter> interpreter (new httpInterpreter(receivedRequest));
    string requestedFile;
    if(interpreter->interpretRequest(requestedFile))
    {
        string content, contType;
        if(reader.getFile(requestedFile, content, contType))
        {
            interpreter->constructResponse(content, contType);
        }
        string message = interpreter->getResponse();
        // Send message
        send(clientSocket, message.c_str(), message.length()+1, 0);
    }

    //Close client-socket
    if(clientSocket != INVALID_SOCKET)
        #ifdef WINDOWS
        closesocket(clientSocket);
        #else
        close(clientSocket);
        #endif
}

bool webServer::setDirectory(string &dir)
{
    return reader.setDirectory(dir);
}
