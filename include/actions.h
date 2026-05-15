#ifndef ACTIONS_H
#define ACTIONS_H

#include <mysql/mysql.h>

void runMenu(MYSQL* conn);
void userAccount(MYSQL* conn, int id);

#endif