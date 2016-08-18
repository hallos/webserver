#include "server.h"


//--------------------------------------------
// startServer
//--------------------------------------------
bool webServer::startServer()
{
    if(run){
        std::cout << "Server already running.." << std::endl;
        return false;
    }
    else{
        run = true;
        runServer(80);
        return true;
    }
}
//--------------------------------------------
// stopServer
// Takes an bool-pointer and sets the bool to false
// Meant to be used to stop the server from another thread
//--------------------------------------------
void webServer::stopServer()
{
    run = false;
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

            while(run){
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
    char sendBuffer[150] = "HTTP/1.0 200 OK\nDate: Fri, 17 Jun 2015 23:59:59 GMT\nContent-Type: text/html\nContect-Length: 140\n\n<html>\n<body>\nHello world!\n</body>\n</html>";
    send(clientSocket, sendBuffer, 150, 0);
    //Close client-socket
    if(clientSocket != INVALID_SOCKET)
        closesocket(clientSocket);
}
