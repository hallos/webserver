#ifndef WEBCONNECTIONHANDLER_H_INCLUDED
#define WEBCONNECTIONHANDLER_H_INCLUDED

#include "Server/Server.h"
#include "FileReader/FileReader.h"

class WebConnectionHandler : public ConnectionHandler
{
public:
    WebConnectionHandler(std::shared_ptr<FileReader> fileReader): fileReader_(fileReader) {};
    ~WebConnectionHandler() {};

    void onAccept(std::shared_ptr<ITCPStreamSocket> socket);
private:
    std::shared_ptr<FileReader> fileReader_;
};

#endif