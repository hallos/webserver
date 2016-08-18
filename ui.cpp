#include "ui.h"
#include <iostream>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
//--------------------------------------------
// printMainMenu
//--------------------------------------------
void printMainMenu(bool running)
{

    //clearScreen();
    cout << "WEBSERVER   -  ";
    if(running){
        cout << " LIVE: " << running << endl;
    }
    cout << endl << endl
            << "1. Start server" << endl
            << "2. Stop server" << endl
            << "0. EXIT" << endl;
}
//--------------------------------------------
// clearScreen
// clears windows console-window
//--------------------------------------------
void clearScreen()
{
    system("cls");
}
//--------------------------------------------
//  getMenuOption
//  Asks user for a integer as a menuoption
//  Makes sure input is valid and within range
//  of available options.
//  Returns the options as an integer and takes
//  number of valid options from 0 and up as argument
//--------------------------------------------
int getMenuOption(int maxOptions)
{
    int tmpOption=0;
    bool inputOK=false;
    cout << "Enter menuoption: ";
    cin.exceptions(std::istream::failbit | std::istream::badbit);
    do{
        try{
            cin >> tmpOption;
            cin.get();

            if(tmpOption>=0 && tmpOption<maxOptions){
                inputOK=true;
            }
            else{
                inputOK=false;
                throw std::range_error("Option out of range");
            }
        }
        catch(std::exception){
            std::cerr << "Bad input try again: ";
            if(!cin){
                cin.clear();
                cin.ignore(999,'\n');
                tmpOption=0;
            }
        }
    }while(!inputOK);
    return tmpOption;
}
