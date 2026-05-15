#ifndef MENU_H
#define MENU_H

#include <mysql/mysql.h>

void drawMainMenu();
void drawAccountMenu(int id);
int promptChoice();
double getBalanceInput();
int getPinInput();

#endif