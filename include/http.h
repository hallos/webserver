#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#include <string>

using namespace std;

class httpInterpreter
{
    private:
        string incomingMSG;
        string outgoingMSG;
        string requestType;
        string fileContent;

        string getTimeStamp();

    public:
        httpInterpreter() {} //Default constructor
        ~httpInterpreter() {} //Default deconstructor

        bool isHTTP();
        bool interpretRequest(string recMessage, string &sendMessage);


};

#endif // HTTP_H_INCLUDED
