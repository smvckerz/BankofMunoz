#ifndef MENU_H
#define MENU_H

#include <mysql/mysql.h>

void runMenu(MYSQL* conn);
void userAccount(MYSQL* conn, int id);

#endif