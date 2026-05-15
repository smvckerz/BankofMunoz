#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>

MYSQL* connectDatabase();
void disconnectDatabase(MYSQL* conn);
bool createAccount(MYSQL* conn, const std::string& name, double balance, int pin);
bool authenticateAccount(MYSQL* conn, int id, int pin);
void getBalance(MYSQL* conn, int id);
void viewAccounts(MYSQL* conn);

#endif