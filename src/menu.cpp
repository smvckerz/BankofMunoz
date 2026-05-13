#include "menu.h"
#include "database.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

static int getChoice()
{
    int choice;

    while (true)
    {
        cout << "============== MENU ==============\n";
        cout << "1. Create Account\n";
        cout << "2. View Accounts\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";

        if (cin >> choice)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cout << "Invalid input. Enter a number.\n\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static double getBalanceInput()
{
    double balance;

    while (true)
    {
        cout << "Enter starting balance: ";

        if (cin >> balance)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (balance >= 0.0)
            {
                return balance;
            }

            cout << "Balance cannot be negative.\n";
            continue;
        }

        cout << "Invalid balance. Enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

static void handleCreateAccount(MYSQL* conn)
{
    string name;
    cout << "Enter account holder name: ";
    getline(cin, name);

    if (name.empty())
    {
        cout << "Name cannot be empty.\n\n";
        return;
    }

    double balance = getBalanceInput();
    createAccount(conn, name, balance);
    cout << endl;
}

void runMenu(MYSQL* conn)
{
    bool running = true;

    while (running)
    {
        int choice = getChoice();
        cout << endl;

        switch (choice)
        {
            case 1:
                handleCreateAccount(conn);
                break;

            case 2:
                viewAccounts(conn);
                break;

            case 3:
                cout << "Exiting BankCore. Goodbye.\n";
                running = false;
                break;

            default:
                cout << "Invalid option. Try again.\n\n";
                break;
        }
    }
}