#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "server.h"

/** \brief A user interface module with functions for operating\
 *  webServer objects
 *
 */


void printMainMenu(bool running);
int getMenuOption(int maxOptions);
void clearScreen();
void chooseDirectory(webServer &server);

#endif // UI_H_INCLUDED
