#include "database.h"
#include "config.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>

using namespace std;

static string escapeString(MYSQL* conn, const string& input)
{
    string escaped;
    escaped.resize(input.length() * 2 + 1);

    unsigned long escapedLength = mysql_real_escape_string(
        conn,
        &escaped[0],
        input.c_str(),
        input.length()
    );

    escaped.resize(escapedLength);
    return escaped;
}

MYSQL* connectDatabase()
{
    MYSQL* conn = mysql_init(nullptr);

    if (!conn)
    {
        cerr << "mysql_init failed." << endl;
        return nullptr;
    }

    if (!mysql_real_connect(conn, "localhost", "root", DB_PASSWORD, DB_NAME, 0, nullptr, 0))
    {
        cerr << "Connection failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return nullptr;
    }

    cout << "Connected to BankCore database successfully." << endl;
    return conn;
}

void disconnectDatabase(MYSQL* conn)
{
    if (conn != nullptr)
    {
        mysql_close(conn);
    }
}

bool createAccount(MYSQL* conn, const string& name, double balance)
{
    if (conn == nullptr)
    {
        cerr << "Database connection is null." << endl;
        return false;
    }

    string escapedName = escapeString(conn, name);

    string query = "INSERT INTO accounts (name, balance) VALUES ('" +
                   escapedName + "', " +
                   to_string(balance) + ")";

    if (mysql_query(conn, query.c_str()))
    {
        cerr << "Error creating account: " << mysql_error(conn) << endl;
        return false;
    }

    cout << fixed << setprecision(2);
    cout << "Account created for " << name
         << " with balance $" << balance << endl;

    return true;
}

void viewAccounts(MYSQL* conn)
{
    if (conn == nullptr)
    {
        cerr << "Database connection is null." << endl;
        return;
    }

    string query = "SELECT id, name, balance FROM accounts ORDER BY id";

    if (mysql_query(conn, query.c_str()))
    {
        cerr << "Error retrieving accounts: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);

    if (result == nullptr)
    {
        cerr << "Error storing result: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_ROW row;

    cout << "\n================ ACCOUNT LIST ================\n";
    cout << left << setw(10) << "ID"
         << setw(25) << "Name"
         << setw(15) << "Balance" << endl;
    cout << "----------------------------------------------\n";

    while ((row = mysql_fetch_row(result)))
    {
        cout << left << setw(10) << (row[0] ? row[0] : "")
             << setw(25) << (row[1] ? row[1] : "")
             << setw(15) << (row[2] ? row[2] : "") << endl;
    }

    cout << "==============================================\n\n";

    mysql_free_result(result);
}