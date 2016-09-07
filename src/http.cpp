#include "http.h"
#include "../server.h"
#include <iostream>
#include <time.h>

using namespace std;

bool interpretRequest(webServer *server, string recMessage, string &sendMessage)
{
    cout << "Incoming request: " << endl << recMessage << endl;
    string firstLine;
    //Get first line of received message
    firstLine = recMessage.substr(0,recMessage.find_first_of('\n'));
    //Control that it is a HTTP-request
    if( firstLine.find("HTTP",0) != string::npos ){
        cout << "HTTP-request found!" << endl;

        string::size_type position;
        if( position = firstLine.find("HEAD",0) != string::npos )
        {
            cout << "HEAD received!" << endl;
        }
        else if( position = firstLine.find("GET",0) != string::npos )
        {
            //Get positions for filename
            string::size_type fileNameBegin = position + 3;
            string::size_type fileNameEnd = firstLine.find(" ", fileNameBegin) - fileNameBegin;
            //Extract filename
            string requestedFile = firstLine.substr(fileNameBegin, fileNameEnd);
            cout << "GET received! Requesting: " << requestedFile << endl;
        }
        return true;
    }

    return false;

    /*
    if("http" is in firstline-string){
        std::string tmp;
        for(int i=0; firstLine[i]!=" "; i++) tmp += firstLine[i];

        switch(tmp)
        {
        case "GET":
            break;
        case "HEAD":
            break;
        default:
            break;
        }
    }
    */
}

string httpGET(string file)
{

}

string getTimeStamp()
{
    return "ret";
}
