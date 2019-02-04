#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#include <string>

namespace HTTP
{
    enum HTTPType
    {
        UNAVAILABLE,
        OK,
        GET,
        POST,
        HEAD
    };

    HTTPType httpType(std::string msg);
    bool isValidHTTP(std::string msg);
    std::string getTimeStamp();
    int getContentLength(std::string reqHeader);
    std::string constructBadRequestResponse();
    std::string constructNotFoundResponse(const std::string& fileName);
    std::string constructContinueResponse();
    std::string constructOKResponse(const std::string& fileContent, const std::string& contentType);
    std::string interpretGETRequest(std::string getRequest);
    std::string interpretHEADRequest(std::string headRequest);
};

#endif // HTTP_H_INCLUDED
