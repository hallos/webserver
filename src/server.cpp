#include "..\include\server.h"
#include "..\include\http.h"

#include <fstream>
#include <sstream>

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
// setDirectory
//--------------------------------------------
bool webServer::setDirectory(string &dir)
{
    dirMutex.lock();
    //Save old directory
    string oldDir = directory;
    //Set directory string
    directory = dir;
    dirMutex.unlock();
    //Try to buffer index-file to string indexFile
    if(bufferIndexFile()){
        return true;
    }
    //Set directory as old-directory if buffering fails
    dirMutex.lock();
    directory = oldDir;
    dirMutex.unlock();
    return false;
}
//--------------------------------------------
// getDirectory
//--------------------------------------------
string webServer::getDirectory()
{
    dirMutex.lock();
    string tmp = directory;
    dirMutex.unlock();
    return tmp;
}
//--------------------------------------------
// setIndexBuffer
//--------------------------------------------
bool webServer::setIndexBuffer(string &index)
{
    bufferMutex.lock(); //Wait for access to indexBuffer
    indexBuffer = index;
    bufferMutex.unlock(); //Release indexBuffer
    return true;
}
//--------------------------------------------
// getIndexBuffer
//--------------------------------------------
string webServer::getIndexBuffer(int &length)
{
    bufferMutex.lock(); //Wait for access to indexBuffer
    length = indexBuffer.length();
    string retString = indexBuffer;
    bufferMutex.unlock(); //Release indexBuffer
    return retString;
}
//--------------------------------------------
// bufferIndexFile
//--------------------------------------------
bool webServer::bufferIndexFile()
{
    fstream file;
    file.exceptions( ifstream::failbit | ifstream::badbit );

    try{
        //Open index-file and read the file to string indexFile
        file.open(this->getDirectory()+"index.html");
        string tmp, readFromFile;

        while(!file.eof()){
            getline(file, tmp);
            readFromFile += tmp + "\n";
        }
        this->setIndexBuffer(readFromFile);

        return true;
    }
    catch(...){
        cerr << "index.html missing..";
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
        }

        //Clean up WinSock
        if(WSACleanup() != 0)
        {
            cerr << "Cleanup failed!\n";
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
    string message;
    httpParser.interpretRequest(recBuffer, message);

    //Get buffered index-file and its length
    int  contentLength = 0;
    string content = getIndexBuffer(contentLength);

    string header = "HTTP/1.0 200 OK\nDate: Fri, 17 Jun 2015 23:59:59 GMT\nContent-Type: text/html\nContent-Length: " + to_string(contentLength) + "\n\n";
    message = header + content;

    int bufferLength = message.length();
    char * sendBuffer = new char[bufferLength]();

    cout << "sendBuffer:" << endl;
    //Copy message-string to char buffer
    for(int i=0; i<bufferLength; i++){
        sendBuffer[i] = message[i];
        cout << sendBuffer[i];
    }

    send(clientSocket, sendBuffer, bufferLength, 0);

    delete sendBuffer;
    //Close client-socket
    if(clientSocket != INVALID_SOCKET)
        closesocket(clientSocket);
}
