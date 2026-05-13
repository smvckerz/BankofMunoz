#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>

MYSQL* connectDatabase();
void disconnectDatabase(MYSQL* conn);

bool createAccount(MYSQL* conn, const std::string& name, double balance);
void viewAccounts(MYSQL* conn);

#endif