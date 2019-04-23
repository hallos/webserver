#include "HTTP.h"
#include <string> //toString()
#include <sstream> //ostringstream
#include <iomanip>
#include <time.h>


HTTP::HTTPType HTTP::httpType(std::string msg)
{
    std::string firstLine = msg.substr(0,msg.find_first_of('\n'));

    if (firstLine.find("OK",0) != std::string::npos)
    {
        return HTTPType::OK;
    }
    else if (firstLine.find("GET",0) != std::string::npos)
    {
        return HTTPType::GET;
    }
    else if (firstLine.find("POST",0) != std::string::npos)
    {
        return HTTPType::POST;
    }
    
    return HTTPType::UNAVAILABLE;
}

bool HTTP::isValidHTTP(std::string msg)
{
    std::string firstLine = msg.substr(0,msg.find_first_of('\n'));

    return (firstLine.find("HTTP",0) != std::string::npos);
}

std::string HTTP::constructOKResponse(const std::string& fileContent, const std::string& contentType)
{
    std::ostringstream ss;
    ss << "HTTP/1.0 200 OK\r\n"
       << "Date: " << HTTP::getTimeStamp() << "\r\n"
       << "Server: webServer/1.0\r\n"
       << "Content-type: " << contentType << "\r\n"
       << "Content-length: " << fileContent.length() << "\r\n"
       << "\r\n"
       << fileContent;
    
    return ss.str();
}

int HTTP::getContentLength(std::string reqHeader)
{
    std::string searchPattern = "Content-Length: ";
    auto lengthPos = reqHeader.find(searchPattern);
    if (lengthPos == std::string::npos)
        return 0;
    auto lengthEnd = reqHeader.find("\r\n", lengthPos);
    if (lengthEnd == std::string::npos)
        return 0;

    std::string length = reqHeader.substr(lengthPos + searchPattern.length(), lengthEnd);
    return std::stoi(length);
}

std::string HTTP::constructBadRequestResponse()
{
    std::ostringstream ss;
    ss << "HTTP/1.0 400 Bad Request\r\n"
       << "Date: " << HTTP::getTimeStamp() << "\r\n"
       << "Server: webServer/1.0\r\n"
       << "Content-length: 0\r\n"
       << "\r\n";

    return ss.str();
}

std::string HTTP::constructNotFoundResponse(const std::string& fileName)
{
    std::ostringstream ss;
    ss << "HTTP/1.0 404 Not Found\r\n"
       << "Date: " << HTTP::getTimeStamp() << "\r\n"
       << "Server: webServer/1.0\r\n"
       << "Content-length: 0\r\n"
       << "\r\n";

    return ss.str();
}

std::string HTTP::constructContinueResponse()
{
    std::ostringstream ss;
    ss << "HTTP/1.0 100 Continue\r\n"
       << "Date: " << HTTP::getTimeStamp() << "\r\n"
       << "Server: webServer/1.0\r\n"
       << "Content-length: 0\r\n"
       << "\r\n";

    return ss.str();
}

std::string HTTP::interpretGETRequest(std::string getRequest)
{
    std::string firstLine = getRequest.substr(0, getRequest.find_first_of('\n'));
    //Get positions for filename in request
    std::string::size_type fileNameBegin = firstLine.find("GET",0) + 4;
    std::string::size_type fileNameEnd = firstLine.find(" ", fileNameBegin ) - fileNameBegin;
    //Extract filename
    std::string filename = firstLine.substr(fileNameBegin, fileNameEnd);
    //Erase /-caracter in front of filename
    if (filename[0] == '/') filename.erase(0,1);
    if (filename.empty()) filename = "index.html";

    return filename;
}

std::string HTTP::getTimeStamp()
{
    time_t now;
    struct tm * date;

    time(&now);
    date = gmtime(&now);

    //SYSTEMTIME st;
    //GetSystemTime(&st);

    std::string timeStamp;
    //Append weekday to string
    switch(date->tm_wday/*st.wDayOfWeek*/)
    {
    case 0:
        timeStamp.append("Sun, ");
        break;
    case 1:
        timeStamp.append("Mon, ");
        break;
    case 2:
        timeStamp.append("Tue, ");
        break;
    case 3:
        timeStamp.append("Wed, ");
        break;
    case 4:
        timeStamp.append("Thu, ");
        break;
    case 5:
        timeStamp.append("Fri, ");
        break;
    case 6:
        timeStamp.append("Sat, ");
        break;
    }
    //Append day in month to string
    timeStamp.append(std::to_string(date->tm_mday/*st.wDay*/) + " ");
    //Append month to string
    switch(date->tm_mon/*st.wMonth*/)
    {
    case 1:
        timeStamp.append("Jan");
        break;
    case 2:
        timeStamp.append("Feb");
        break;
    case 3:
        timeStamp.append("Mar");
        break;
    case 4:
        timeStamp.append("Apr");
        break;
    case 5:
        timeStamp.append("May");
        break;
    case 6:
        timeStamp.append("Jun");
        break;
    case 7:
        timeStamp.append("Jul");
        break;
    case 8:
        timeStamp.append("Aug");
        break;
    case 9:
        timeStamp.append("Sep");
        break;
    case 10:
        timeStamp.append("Oct");
        break;
    case 11:
        timeStamp.append("Nov");
        break;
    case 12:
        timeStamp.append("Dec");
        break;
    }
    //Append year to string
    timeStamp.append(" " + std::to_string(date->tm_year+1900/*st.wYear*/) + " ");
    //Append clock-time to string
    std::ostringstream oss;
    oss << std::internal << std::setfill('0') << std::setw(2) << date->tm_hour /*st.wHour*/ << ":" << std::setw(2) << date->tm_min /*st.wMinute*/ << ":" << std::setw(2) << date->tm_sec /*st.wSecond*/;
    timeStamp.append(oss.str());
    timeStamp.append(" GMT");

    return timeStamp;
}