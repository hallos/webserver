#include "http.h"
#include "../server.h"
#include <iostream>
#include <time.h>

bool interpretRequest(webServer *server, char *recMessage, std::string &sendMessage)
{
    std::cout << "Incoming request: " << std::endl << recMessage << std::endl;
    std::string firstLine;
    //Get first line
    for(int i=0; recMessage[i]!='\n' || recMessage[i]!='\0'; i++) firstLine += recMessage[i];
    //Control that it is a HTTP-request
    if( firstLine.find("HTTP",0) != std::string::npos ){
        std::cout << "HTTP-request found!" << std::endl;


    }

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

std::string httpGET(std::string file)
{

}

std::string getTimeStamp()
{
    return "ret";
}
