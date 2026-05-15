#include "menu.h"
#include "colors.h"
#include <iostream>
#include <limits>

using namespace std;
using namespace color;

// ---------------------------------------------------------------------------
// Menu drawing
// ---------------------------------------------------------------------------

void drawMainMenu()
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

void drawAccountMenu(int id)
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

int promptChoice()
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

double getBalanceInput()
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

int getPinInput()
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