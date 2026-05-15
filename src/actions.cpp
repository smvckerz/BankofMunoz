#include "actions.h"
#include "menu.h"        // ← needs this to call drawMainMenu, promptChoice, etc.
#include "database.h"
#include "colors.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;
using namespace color;

static void handleCreateAccount(MYSQL* conn)
{
    string name;
    cout << B_GREEN << "  ➜ " << RESET << "Enter account holder name: ";
    getline(cin, name);

    if (name.empty())
    {
        cout << B_RED << "  ✗ " << RESET << "Name cannot be empty.\n\n";
        return;
    }

    double balance = getBalanceInput();
    int pin        = getPinInput();
    createAccount(conn, name, balance, pin);
    cout << endl;
}

static void handleLogin(MYSQL* conn)
{
    int id;
    cout << B_GREEN << "  ➜ " << RESET << "Enter Account ID: ";
    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << B_RED << "  ✗ " << RESET << "Invalid ID.\n\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int pin;
    cout << B_GREEN << "  ➜ " << RESET << "Enter PIN: ";
    if (!(cin >> pin))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << B_RED << "  ✗ " << RESET << "Invalid PIN.\n\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl;

    if (authenticateAccount(conn, id, pin))
    {
        cout << B_GREEN << "  ✓ " << RESET << "Access granted. Welcome.\n";
        userAccount(conn, id);
    }
    else
    {
        cout << B_RED << "  ✗ " << RESET << "Access denied.\n\n";
    }
}

// ---------------------------------------------------------------------------
// Menus
// ---------------------------------------------------------------------------

void userAccount(MYSQL* conn, int id)
{
    bool running = true;
    while (running)
    {
        drawAccountMenu(id);
        int choice = promptChoice();
        cout << endl;

        switch (choice)
        {
            case 1: getBalance(conn, id);                                       break;
            case 2: cout << DIM << "  Change PIN — not yet implemented.\n\n" << RESET; break;
            case 3: cout << DIM << "  Transfer Money — not yet implemented.\n\n" << RESET; break;
            case 4:
                cout << B_GREEN << "  ✓ " << RESET << "Logged out of account #" << id << ".\n";
                running = false;
                break;
            default:
                cout << B_RED << "  ✗ " << RESET << "Invalid option. Try again.\n\n";
                break;
        }
    }
}

void runMenu(MYSQL* conn)
{
    bool running = true;
    while (running)
    {
        drawMainMenu();
        int choice = promptChoice();
        cout << endl;

        switch (choice)
        {
            case 1: handleCreateAccount(conn); break;
            case 2: handleLogin(conn);         break;
            case 3: viewAccounts(conn);        break;
            case 4:
                cout << B_GREEN << "  ✓ " << RESET << "Exiting BankCore. Goodbye.\n";
                running = false;
                break;
            default:
                cout << B_RED << "  ✗ " << RESET << "Invalid option. Try again.\n\n";
                break;
        }
    }
}