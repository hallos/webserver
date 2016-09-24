#include "../include/http.h"
#include <iostream>
#include <time.h>

using namespace std;

/** \brief Tests if httpInterpeter actually received a HTTP-request
 *
 * \return bool True if receivedMSG is a HTTP-request, False otherwise
 *
 */
bool httpInterpreter::isHTTP()
{
    //Get first line of received incoming message
    string firstLine;
    firstLine = receivedMSG.substr(0,receivedMSG.find_first_of('\n'));
    //Test if it is a HTTP-request and return true if that's the case
    if( firstLine.find("HTTP",0) != string::npos )
        return true;
    //Return false if received message was not a HTTP-request
    return false;
}

/** \brief Interprets HTTP-request and extracts filename of requested file
 *
 * \param &filename Reference to a string where extracted filename is to be saved
 * \return bool True if request was successfully interpreted, False otherwise
 *
 */
bool httpInterpreter::interpretRequest(string &filename)
{
    //Control that it is a HTTP-request
    if( isHTTP() ){
        //Get first line of received message
        string firstLine;
        firstLine = receivedMSG.substr(0,receivedMSG.find_first_of('\n'));

        //Detect type of HTTP-request
        bool validRequest = false;
        string::size_type position; //Keeps track of postion in string firstLine
        if( (position = firstLine.find("HEAD",0)) != string::npos )
        {
            requestType = "HEAD";
            validRequest = true;
        }
        else if( (position = firstLine.find("GET",0)) != string::npos )
        {
            requestType = "GET";
            validRequest = true;
        }
        //Control that HTTP-request is valid and extract requested filename
        if(validRequest)
        {
            //Get positions for filename in request
            string::size_type fileNameBegin = position + requestType.length();
            string::size_type fileNameEnd = firstLine.find(" ", fileNameBegin+1 ) - fileNameBegin;
            //Extract filename
            filename = firstLine.substr(fileNameBegin, fileNameEnd);
            return true;
        }
    }
    return false;
}


string httpInterpreter::getTimeStamp()
{
    return "ret";
}
