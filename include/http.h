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
        void constructHead(const string &contentType);

    public:
        httpInterpreter(string &recMsg): receivedMSG(recMsg) {} //Default constructor
        ~httpInterpreter() {} //Default deconstructor

        bool isHTTP();
        bool interpretRequest(string &filename);
        bool constructResponse(const string &file, const string &contentType);
        string getResponse();


};

#endif // HTTP_H_INCLUDED
