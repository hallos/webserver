#ifndef WEBCONNECTIONHANDLER_H_INCLUDED
#define WEBCONNECTIONHANDLER_H_INCLUDED

#include <tcp_server.h>
#include "FileReader/FileReader.h"

class WebConnectionHandler : public connection_handler
{
public:
    WebConnectionHandler(std::shared_ptr<FileReader> fileReader): fileReader_(fileReader) {};
    ~WebConnectionHandler() {};

    void onAccept(std::shared_ptr<Itcp_stream_socket> socket);
private:
    std::shared_ptr<FileReader> fileReader_;
};

#endif