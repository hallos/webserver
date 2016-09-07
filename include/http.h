#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

bool interpretRequest(webServer *server, std::string recMessage, std::string &sendMessage);
std::string httpGET(std::string file);
std::string getTimeStamp();

#endif // HTTP_H_INCLUDED
