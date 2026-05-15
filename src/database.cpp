#include "database.h"
#include "config.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Returns the value of `name` from the environment, or `fallback` if unset/empty.
static const char* envOr(const char* name, const char* fallback)
{
    const char* value = std::getenv(name);
    return (value && *value) ? value : fallback;
}

// Same idea for an unsigned int port.
static unsigned int envOrUInt(const char* name, unsigned int fallback)
{
    const char* value = std::getenv(name);
    if (value && *value)
    {
        return static_cast<unsigned int>(std::atoi(value));
    }
    return fallback;
}

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

// ---------------------------------------------------------------------------
// Connection lifecycle
// ---------------------------------------------------------------------------

MYSQL* connectDatabase()
{
    MYSQL* conn = mysql_init(nullptr);

    if (!conn)
    {
        cerr << "mysql_init failed." << endl;
        return nullptr;
    }

    // Runtime env vars take precedence over compile-time defaults in config.h.
    const char*  host     = envOr     ("DB_HOST",     DB_HOST);
    unsigned int port     = envOrUInt ("DB_PORT",     DB_PORT);
    const char*  user     = envOr     ("DB_USER",     DB_USER);
    const char*  password = envOr     ("DB_PASSWORD", DB_PASSWORD);
    const char*  dbname   = envOr     ("DB_NAME",     DB_NAME);

    if (!mysql_real_connect(conn, host, user, password, dbname, port, nullptr, 0))
    {
        cerr << "Connection failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return nullptr;
    }

    cout << "Connected to BankCore database (" << host << ":" << port << ") successfully." << endl;
    return conn;
}

void disconnectDatabase(MYSQL* conn)
{
    if (conn != nullptr)
    {
        mysql_close(conn);
    }
}

// ---------------------------------------------------------------------------
// Account operations
// ---------------------------------------------------------------------------

bool createAccount(MYSQL* conn, const string& name, double balance, int pin)
{
    if (conn == nullptr)
    {
        cerr << "Database connection is null." << endl;
        return false;
    }

    string escapedName = escapeString(conn, name);

    string query = "INSERT INTO accounts (name, balance, pin) VALUES ('" +
                   escapedName + "', " +
                   to_string(balance) + ", " + to_string(pin) + ")";

    if (mysql_query(conn, query.c_str()))
    {
        cerr << "Error creating account: " << mysql_error(conn) << endl;
        return false;
    }

    cout << fixed << setprecision(2);
    cout << "Account created for " << name
         << " with balance $" << balance << " and pin: " << pin << endl;

    return true;
}

void getBalance(MYSQL* conn, int id)
{
    if (conn == nullptr)
    {
        cerr << "Database connection is null." << endl;
        return;
    }

    string query = "SELECT balance FROM accounts WHERE id = " + to_string(id);
    if (mysql_query(conn, query.c_str()))
    {
        cerr << "Error retrieving accounts: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);

    if (result == nullptr) 
    {
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (row && row[0])
    {
        cout << "Your current balance: $" << (row[0]) << endl;
    }
    else
    {
        cout << "Account not found." << endl;
    }

    mysql_free_result(result);
}

void viewAccounts(MYSQL* conn)
{
    if (conn == nullptr)
    {
        cerr << "Database connection is null." << endl;
        return;
    }

    string query = "SELECT id, name, balance, pin FROM accounts ORDER BY id";

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
         << setw(15) << "Balance"
         << setw(15) << "Pin" << endl;
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

bool authenticateAccount(MYSQL* conn, int id, int pin)
{
    if (conn == nullptr) 
    {
        return false;
    }

    string query = "SELECT pin FROM accounts WHERE id = " + to_string(id);

    if (mysql_query(conn, query.c_str()))
    {
        cerr << "Query error: " << mysql_error(conn) << endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (result == nullptr) 
    {
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    bool authenticated = false;

    if (row && row[0])
    {
        int storedPin = std::atoi(row[0]);
        authenticated = (storedPin == pin);
    }

    mysql_free_result(result);

    if (!authenticated)
    {
        cerr << "Invalid account ID or PIN." << endl;
    }

    return authenticated;
}