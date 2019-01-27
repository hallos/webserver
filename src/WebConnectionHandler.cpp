#include "WebConnectionHandler.h"
#include "HTTP.h"

void WebConnectionHandler::onAccept(int id, std::shared_ptr<ITCPStreamSocket> socket)
{
    std::string request = socket->receiveData();
    auto headerLength = request.find("\r\n\r\n");
    if (headerLength == std::string::npos)
    {
        // Respond to bad request
    }
    std::string reqHeader = request.substr(0, headerLength);
    std::string reqBody = request.substr(headerLength+4, request.length());
    // Interpret header
    HTTP::HTTPType requestType = HTTP::httpType(reqHeader);
    // Construct response if HEAD or GET request
    if (HTTP::HTTPType::GET)
    {
        std::string requestedFile = HTTP::interpretGETRequest(reqHeader);
        auto file = fileReader_->getFile(requestedFile);
        std::string response;
        if (file)
        {
            response = HTTP::constructOKResponse(file->getContent(), file->getContentType());
        }
        else
        {
            response = HTTP::constructNotFoundResponse(requestedFile);
        }
        socket->sendData(response);
    }
    // Check Expect header for 100-continue on POST request
    // Fetch content-length and then reqBody if request is a POST.
    int contentLength = 0;
    int receivedContent = reqBody.length();
    while (contentLength > receivedContent)
    {
        int bytesMissing = contentLength - receivedContent;
        std::string data = socket->receiveData();
        if (data.length() > bytesMissing)
        {
            reqBody.append(data, 0, bytesMissing);
        }
        else
        {
            reqBody.append(data);
        }
    }
}