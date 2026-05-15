#include "menu.h"
#include "database.h"
#include "colors.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;
using namespace color;

// ---------------------------------------------------------------------------
// Menu drawing
// ---------------------------------------------------------------------------

static void drawMainMenu()
{
    cout << "\n"
         << CYAN << "  ╭──────────────────────────────────╮\n"
         << CYAN << "  │         " << B_YELLOW << "B A N K C O R E" << CYAN << "          │\n"
         << CYAN << "  ├──────────────────────────────────┤\n"
         << CYAN << "  │                                  │\n"
         << CYAN << "  │   " << B_CYAN << "❯ 1" << RESET << B_WHITE << "   Create Account           " << CYAN << "│\n"
         << CYAN << "  │   " << B_CYAN << "❯ 2" << RESET << B_WHITE << "   Login                    " << CYAN << "│\n"
         << CYAN << "  │   " << B_CYAN << "❯ 3" << RESET << B_WHITE << "   View All Accounts        " << CYAN << "│\n"
         << CYAN << "  │   " << B_CYAN << "❯ 4" << RESET << DIM     << "   Exit                     " << CYAN << "│\n"
         << CYAN << "  │                                  │\n"
         << CYAN << "  ╰──────────────────────────────────╯\n"
         << RESET;
}

static void drawAccountMenu(int id)
{
    cout << "\n"
         << DIM  << "  Logged in as account #" << id << "\n" << RESET
         << "\n"
         << CYAN << "  ╭──────────────────────────────────╮\n"
         << CYAN << "  │          " << B_YELLOW << "A C C O U N T" << CYAN << "           │\n"
         << CYAN << "  ├──────────────────────────────────┤\n"
         << CYAN << "  │                                  │\n"
         << CYAN << "  │   " << B_CYAN << "❯ 1" << RESET << B_WHITE << "   View Balance             " << CYAN << "│\n"
         << CYAN << "  │   " << B_CYAN << "❯ 2" << RESET << B_WHITE << "   Change PIN               " << CYAN << "│\n"
         << CYAN << "  │   " << B_CYAN << "❯ 3" << RESET << B_WHITE << "   Transfer Money           " << CYAN << "│\n"
         << CYAN << "  │   " << B_CYAN << "❯ 4" << RESET << DIM     << "   Logout                   " << CYAN << "│\n"
         << CYAN << "  │                                  │\n"
         << CYAN << "  ╰──────────────────────────────────╯\n"
         << RESET;
}

static int promptChoice()
{
    int choice;
    while (true)
    {
        cout << B_GREEN << "  ➜ " << RESET << "Choose an option: ";
        if (cin >> choice)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cout << B_RED << "  ✗ " << RESET << "Invalid input. Enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ---------------------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------------------

static double getBalanceInput()
{
    double balance;
    while (true)
    {
        cout << B_GREEN << "  ➜ " << RESET << "Enter starting balance: $";
        if (cin >> balance)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (balance >= 0.0) return balance;
            cout << B_RED << "  ✗ " << RESET << "Balance cannot be negative.\n";
            continue;
        }
        cout << B_RED << "  ✗ " << RESET << "Invalid balance. Enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static int getPinInput()
{
    int pin;
    while (true)
    {
        cout << B_GREEN << "  ➜ " << RESET << "Set 4-digit PIN: ";
        if (cin >> pin && pin >= 1000 && pin <= 9999)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return pin;
        }
        cout << B_RED << "  ✗ " << RESET << "PIN must be a number between 1000 and 9999.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ---------------------------------------------------------------------------
// Action handlers
// ---------------------------------------------------------------------------

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