#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

using namespace std;

bool interpretRequest(webServer *server, string recMessage, string &sendMessage);
string httpGET(string file);
string getTimeStamp();

#endif // HTTP_H_INCLUDED
