#include "ui.h"
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
            << "0. EXIT" << endl;
}
/** \brief Clears windows console window
 *
 * \return void
 *
 */
void clearScreen()
{
    #ifdef WINDOWS
    system("cls");
    #else
    system("clear");
    #endif
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