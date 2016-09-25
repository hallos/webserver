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
        void constructHead();

    public:
        httpInterpreter(string &recMsg): receivedMSG(recMsg) {} //Default constructor
        ~httpInterpreter() {} //Default deconstructor

        bool isHTTP();
        bool interpretRequest(string &filename);
        bool constructResponse(const string &file);
        string getResponse();


};

#endif // HTTP_H_INCLUDED
