#include "server.h"

#include <fstream>
#include <sstream>


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
        std::cout << "Server already running.." << std::endl;
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
// setDirectory
//--------------------------------------------
bool webServer::setDirectory(std::string &dir)
{
    //Set directory string
    directory = dir;
    //Buffer index-file to string indexFile
    return bufferIndexFile();
}
//--------------------------------------------
// getDirectory
//--------------------------------------------
std::string webServer::getDirectory()
{
    return directory;
}
//--------------------------------------------
// setIndexBuffer
//--------------------------------------------
bool webServer::setIndexBuffer(std::string &index)
{
    bufferMutex.lock(); //Wait for access to indexBuffer
    indexBuffer = index;
    bufferMutex.unlock(); //Release indexBuffer
    return true;
}
//--------------------------------------------
// getIndexBuffer
//--------------------------------------------
std::string webServer::getIndexBuffer(int &length)
{
    bufferMutex.lock(); //Wait for access to indexBuffer
    length = indexBuffer.length();
    std::string retString = indexBuffer;
    bufferMutex.unlock(); //Release indexBuffer
    return retString;
}
//--------------------------------------------
// bufferIndexFile
//--------------------------------------------
bool webServer::bufferIndexFile()
{
    std::fstream file;
    file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

    try{
        //Open index-file and read the file to string indexFile
        file.open(this->getDirectory()+"/index.html");
        std::string tmp, readFromFile;

        while(!file.eof()){
            getline(file, tmp);
            readFromFile += tmp + "\n";
        }
        this->setIndexBuffer(readFromFile);

        return true;
    }
    catch(...){
        std::cerr << "index.html missing..";
        return false;
    }
}
//--------------------------------------------
// runServer
// Initialize sockets at given port and starts listening
// to incoming connections with select().
// Each new connection gets handled in a new thread
// with handleConnection
//--------------------------------------------
bool webServer::runServer(int port){
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,0), &wsaData)==0)
    {
        //Check winsock version is 2 or higher
        if(LOBYTE(wsaData.wVersion) >= 2)
        {
            SOCKET serverSocket = INVALID_SOCKET;
            sockaddr_in sockAdr = {0};

            if((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
                std::cerr << "INVALID Socket.";
                return false;
            }

            //Set sockAddr
            sockAdr.sin_family = AF_INET;
            sockAdr.sin_port = htons(port);  //Port-number
            sockAdr.sin_addr.S_un.S_addr = INADDR_ANY; //Any IP-adress for incoming connections

            //Bind socket
            if(bind(serverSocket,reinterpret_cast<sockaddr*>(&sockAdr),sizeof(sockAdr))!=0){
                std::cerr << "Couldn't bind socket.";
                return false;
            }

            //Set socket in listening mode
            if(listen(serverSocket, SOMAXCONN)!=0){
                std::cerr << "Couldn't set socket in listening-mode..";
                return false;
            }

            sockaddr_in clientSockAdr;
            int clientSockSize = sizeof(clientSockAdr);

            std::vector<std::thread*> connectionThreads;
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
                            std::cerr << "Accept connection failed.";
                            return false;
                        }
                        //Open a new thread that handles the connection
                        connectionThreads.push_back(new std::thread(&webServer::handleConnection,this,clientSocket));
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
        }

        //Clean up WinSock
        if(WSACleanup() != 0)
        {
            std::cerr << "Cleanup failed!\n";
        }
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
    std::cout << recBuffer;

    //Get buffered index-file and its length
    std::ostringstream iss;
    int contentLength;
    std::string content = getIndexBuffer(contentLength);
    iss << contentLength;

    std::string header = "HTTP/1.0 200 OK\nDate: Fri, 17 Jun 2015 23:59:59 GMT\nContent-Type: text/html\nContent-Length: " + iss.str() + "\n\n";
    std::string message = header + content;

    int bufferLength = message.length();
    char * sendBuffer = new char[bufferLength]();

    std::cout << "sendBuffer:" << std::endl;
    //Copy message-string to char buffer
    for(int i=0; i<bufferLength; i++){
        sendBuffer[i] = message[i];
        std::cout << sendBuffer[i];
    }

    send(clientSocket, sendBuffer, bufferLength, 0);

    delete sendBuffer;
    //Close client-socket
    if(clientSocket != INVALID_SOCKET)
        closesocket(clientSocket);
}
