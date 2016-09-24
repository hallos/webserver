#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#include <string>

using namespace std;

class httpInterpreter
{
    private:
        string receivedMSG;
        string sendMSG;
        string requestType;
        string fileContent;

        string getTimeStamp();

    public:
        httpInterpreter(string &recMsg): receivedMSG(recMsg) {} //Default constructor
        ~httpInterpreter() {} //Default deconstructor

        bool isHTTP();
        bool interpretRequest(string &filename);


};

#endif // HTTP_H_INCLUDED
