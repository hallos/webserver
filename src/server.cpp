#include "server.h"
#include "http.h"

#include <sstream>
#include <memory>
#include <iostream>
#include <vector>

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
        runServer(80);
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
    #ifdef WINDOWS
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,0), &wsaData)==0)
    {
        //Check winsock version is 2 or higher
        if(LOBYTE(wsaData.wVersion) >= 2)
        {
    #endif
            SOCKET serverSocket = INVALID_SOCKET;
            sockaddr_in sockAdr = {0};

            if((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
                cerr << "INVALID Socket.";
                return false;
            }

            //Set sockAddr
            sockAdr.sin_family = AF_INET;
            sockAdr.sin_port = htons(port);  //Port-number
            sockAdr.sin_addr.S_un.S_addr = INADDR_ANY; //Any IP-adress for incoming connections

            //Bind socket
            if(bind(serverSocket,reinterpret_cast<sockaddr*>(&sockAdr),sizeof(sockAdr))!=0){
                cerr << "Couldn't bind socket.";
                return false;
            }

            //Set socket in listening mode
            if(listen(serverSocket, SOMAXCONN)!=0){
                cerr << "Couldn't set socket in listening-mode..";
                return false;
            }

            sockaddr_in clientSockAdr;
            int clientSockSize = sizeof(clientSockAdr);

            vector<thread*> connectionThreads;
            fd_set fdRead, fdTemp;
            FD_ZERO(&fdRead);
            FD_SET(serverSocket,&fdRead);
            TIMEVAL timeOutTime = {1,0};

            while(this->isRunning()){
                //Copy fd_Set to temp variabel for use with select()
                fdTemp = fdRead;
                if( select(0,&fdTemp,NULL,NULL,&timeOutTime) > 0 ){
                    if(FD_ISSET(serverSocket,&fdTemp)){
                        //Accept connection
                        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientSockAdr),&clientSockSize);
                        if(clientSocket==INVALID_SOCKET){
                            cerr << "Accept connection failed.";
                            return false;
                        }
                        //Open a new thread that handles the connection
                        connectionThreads.push_back(new thread(&webServer::handleConnection,this,clientSocket));
                    }
                }
            }

            //Close server-socket
            if(serverSocket != INVALID_SOCKET)
                closesocket(serverSocket);

            //Join and delete connection-threads
            for(auto i : connectionThreads){
                i->join();
                delete i;
            }
        #ifdef WINDOWS
        }

        //Clean up WinSock
        if(WSACleanup() != 0)
        {
            cerr << "Cleanup failed!\n";
        }
        #endif
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
        closesocket(clientSocket);
}

bool webServer::setDirectory(string &dir)
{
    return reader.setDirectory(dir);
}
