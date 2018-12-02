#include "WebConnectionHandler.h"
#include "http.h"

void WebConnectionHandler::onAccept(int id, std::shared_ptr<ITCPClientSocket> clientSocket)
{
    std::string request = clientSocket->receiveData();
    httpInterpreter interpreter(request);
    std::string requestedFile;
    if (interpreter.interpretRequest(requestedFile))
    {
        auto file = fileReader_->getFile(requestedFile);
        if (file)
        {
            interpreter.constructResponse(file->getContent(), file->getContentType());
        }
        std::string response = interpreter.getResponse();
        clientSocket->sendData(response);
    }
}