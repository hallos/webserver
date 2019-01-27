#include "WebConnectionHandler.h"
#include "http.h"

void WebConnectionHandler::onAccept(int id, std::shared_ptr<ITCPStreamSocket> clientSocket)
{
    std::string request = clientSocket->receiveData();
    auto headerLength = request.find("\r\n\r\n");
    if (headerLength == string::npos)
    {
        // Respond to bad request
    }
    std::string header = request.substr(0, headerLength);
    std::string body = request.substr(headerLength+4, request.length());
    // Interpret header
    // Construct response if HEAD or GET request
    // Check Expect header for 100-continue on POST request
    // Fetch content-length and then body if request is a POST.
    int contentLength = 0;
    int receivedContent = body.length();
    while (contentLength > receivedContent)
    {
        int bytesMissing = contentLength - receivedContent;
        std::string data = clientSocket->receiveData();
        if (data.length() > bytesMissing)
        {
            body.append(data, 0, bytesMissing);
        }
        else
        {
            body.append(data);
        }
    }
/*
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
    */
}