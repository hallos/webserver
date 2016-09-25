#include "../include/http.h"
#include <iostream>
#include <string> //toString()
#include <sstream> //ostringstream
#include <iomanip>
#include <windows.h> //SYSTEMTIME

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
 * \param filename string& Reference to a string where extracted filename is to be saved
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
            string::size_type fileNameBegin = position + requestType.length() + 1;
            string::size_type fileNameEnd = firstLine.find(" ", fileNameBegin ) - fileNameBegin;
            //Extract filename
            filename = firstLine.substr(fileNameBegin, fileNameEnd);
            //Erase /-caracter in front of filename
            if(filename[0] == '/') filename.erase(0,1);
            if(filename.empty()) filename = "index.html";

            return true;
        }
    }
    return false;
}

/** \brief Constructs and adds the HTTP-header to sendMSG string.
 * calls getTimeStamp()
 * \return void
 *
 */
void httpInterpreter::constructHead()
{
    sendMSG = "HTTP/1.0 200 OK\n";
    sendMSG.append("Date: " + getTimeStamp() + "\n");
    sendMSG.append("Server: webServer/1.0\n");
    sendMSG.append("Content-type: text/html\n");
    sendMSG.append("Content-length: " + to_string(fileContent.length()) );
    sendMSG.append("\n\n");
}

/** \brief Constructs the response to received HTTP-request.
 * calls constructHead. Returns true if response is successfully
 * constructed, false otherwise
 *
 * \param file const string&
 * \return bool
 *
 */
bool httpInterpreter::constructResponse(const string &file)
{
    fileContent = file;
    constructHead();
    if(requestType == "HEAD") return true; //Return if only the head was requested

    sendMSG.append(fileContent);
    return true;
}

/** \brief Returns the string sendMSG with the HTTP response
 *
 * \return string
 *
 */
string httpInterpreter::getResponse()
{
    return sendMSG;
}

/** \brief Constructs a string with the system time and date
 * in the format WEEKDAY, DAY MONTH YEAR HH:MM:SS in Greenwich Middle Time
 * \return string with system time
 *
 */
string httpInterpreter::getTimeStamp()
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    string timeStamp;
    //Append weekday to string
    switch(st.wDayOfWeek)
    {
    case 0:
        timeStamp.append("Sun, ");
        break;
    case 1:
        timeStamp.append("Mon, ");
        break;
    case 2:
        timeStamp.append("Tue, ");
        break;
    case 3:
        timeStamp.append("Wed, ");
        break;
    case 4:
        timeStamp.append("Thu, ");
        break;
    case 5:
        timeStamp.append("Fri, ");
        break;
    case 6:
        timeStamp.append("Sat, ");
        break;
    }
    //Append day in month to string
    timeStamp.append(to_string(st.wDay) + " ");
    //Append month to string
    switch(st.wMonth)
    {
    case 1:
        timeStamp.append("Jan");
        break;
    case 2:
        timeStamp.append("Feb");
        break;
    case 3:
        timeStamp.append("Mar");
        break;
    case 4:
        timeStamp.append("Apr");
        break;
    case 5:
        timeStamp.append("May");
        break;
    case 6:
        timeStamp.append("Jun");
        break;
    case 7:
        timeStamp.append("Jul");
        break;
    case 8:
        timeStamp.append("Aug");
        break;
    case 9:
        timeStamp.append("Sep");
        break;
    case 10:
        timeStamp.append("Oct");
        break;
    case 11:
        timeStamp.append("Nov");
        break;
    case 12:
        timeStamp.append("Dec");
        break;
    }
    //Append year to string
    timeStamp.append(" " + to_string(st.wYear) + " ");
    //Append clock-time to string
    ostringstream oss;
    oss << internal << setfill('0') << setw(2) << st.wHour << ":" << setw(2) << st.wMinute << ":" << setw(2) << st.wSecond;
    timeStamp.append(oss.str());
    timeStamp.append(" GMT");

    return timeStamp;
}
