#include "..\include\ui.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

using namespace std;


/** \brief Prints main menu in console window
 *
 * \param running
 * \return void
 *
 */
void printMainMenu(bool running)
{
    //clearScreen();
    cout << "WEBSERVER   -  ";
    if(running){
        cout << " LIVE " << endl;
    }
    cout << endl << endl
            << "1. Start server" << endl
            << "2. Stop server" << endl
            << "3. Set directory" << endl
            << "0. EXIT" << endl;
}
/** \brief Clears windows console window
 *
 * \return void
 *
 */
void clearScreen()
{
    system("cls");
}
/** \brief Lets user choose a menu option by inputing an integer value
 *
 * \param maxOptions maximum valid value of option
 * \return int with the chosen option
 *
 */
int getMenuOption(int maxOptions)
{
    int tmpOption=0;
    bool inputOK=false;
    cout << "Enter menuoption: ";
    cin.exceptions(istream::failbit | istream::badbit);
    do{
        try{
            cin >> tmpOption;
            cin.get();

            if(tmpOption>=0 && tmpOption<maxOptions){
                inputOK=true;
            }
            else{
                inputOK=false;
                throw range_error("Option out of range");
            }
        }
        catch(exception){
            cerr << "Bad input try again: ";
            if(!cin){
                cin.clear();
                cin.ignore(999,'\n');
                tmpOption=0;
            }
        }
    }while(!inputOK);
    return tmpOption;
}
/** \brief Lets user choose directory where the index-file is located
 *
 * \param &server a reference to the webServer object
 * \return void
 *
 */
void chooseDirectory(webServer &server)
{
    string directory;
    ifstream read;

    clearScreen();
    //Take user input to set directory where index.html is located
    cout <<  endl << "Set directory for index-file: C:/";
    getline(cin, directory);
    directory = "C:/" + directory + "/";
    //Try to open index.html in directory to control that its a valid directory
    read.open(directory + "index.html");
    if(read){
        cout << "Directory set: " << directory << endl;
        read.close();
        //Set the chosen directory in webServer-object
        server.setDirectory(directory);
    }
    else{
        cout << "Couldn't find " << directory << "index.html" << endl;
    }

    cout << "Press ENTER to return...";
    cin.get();
}
