#ifndef WEBCONNECTIONHANDLER_H_INCLUDED
#define WEBCONNECTIONHANDLER_H_INCLUDED

#include "Server/Server.h"
#include "FileReader/FileReader.h"

class WebConnectionHandler : public ConnectionHandler
{
public:
    WebConnectionHandler(std::shared_ptr<FileReader> fileReader): fileReader_(fileReader) {};
    ~WebConnectionHandler() {};

    void onAccept(int id, std::shared_ptr<ITCPStreamSocket> clientSocket);
private:
    std::shared_ptr<FileReader> fileReader_;
};

#endif