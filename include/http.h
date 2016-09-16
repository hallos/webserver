#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#include <string>

using namespace std;

class httpParser
{
    private:
        string input;
        string getTimeStamp();

    public:
        httpParser() {} //Default constructor
        ~httpParser() {} //Default deconstructor

        bool interpretRequest(string recMessage, string &sendMessage);
        string httpGET(string file);

};

#endif // HTTP_H_INCLUDED
