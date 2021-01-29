#include "WebConnectionHandler.h"
#include "HTTP.h"
#include <iostream>

void WebConnectionHandler::onAccept(std::shared_ptr<hallos::Itcp_stream_socket> socket)
{
    std::string request = socket->receiveData();
    auto headerLength = request.find("\r\n\r\n");
    if (headerLength == std::string::npos)
    {
        // Respond to bad request
    }
    std::string reqHeader = request.substr(0, headerLength);
    std::string reqBody = request.substr(headerLength+4, request.length());
    std::cout << reqHeader << '\n' << reqBody << std::endl;
    // Interpret header
    HTTP::HTTPType requestType = HTTP::httpType(reqHeader);

    if (requestType == HTTP::HTTPType::GET)
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
    else if (requestType == HTTP::HTTPType::HEAD)
    {
        std::string requestedFile = HTTP::interpretHEADRequest(reqHeader);
        auto file = fileReader_->getFile(requestedFile);
        std::string response;
        if (file)
        {
            response = HTTP::constructHEADResponse(file->getContent(), file->getContentType());
        }
        else
        {
            response = HTTP::constructNotFoundResponse(requestedFile);
        }
        socket->sendData(response);
    }
    else if (requestType == HTTP::HTTPType::POST)
    {
        // Check Expect header for 100-continue on POST request
        if (reqHeader.find("Expect: 100-continue") != std::string::npos)
        {
            std::string continueResponse = HTTP::constructContinueResponse();
            socket->sendData(continueResponse);
        }
        // Fetch content-length and then reqBody if request is a POST.
        int contentLength = HTTP::getContentLength(reqHeader);
        std::cout << "Received cont-length: " << contentLength;
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
}