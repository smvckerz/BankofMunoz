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
        cout << "2. Login to Account\n";
        cout << "3. View Accounts\n";
        cout << "4. Exit\n";
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

static int GetPinInput()
{
    int pin;

    while(pin)
    {
        cout << "Set 4-Digit Pin";
        if(cin >> pin && pin >= 1000 && pin <= 9999)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return pin;
        }
        cout << "Pin has to be greater than 1000 and less than 9999, try again." << endl;
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
    int pin = GetPinInput();
    createAccount(conn, name, balance, pin);
    cout << endl;
}

static void handleLogin (MYSQL* conn)
{
    int id;
    cout << "Enter Account ID: ";
    cin >> id;

    if(!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID.\n\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int pin;
    cout << "Enter PIN: ";
    if (!(cin >> pin))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid PIN.\n\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(authenticateAccount(conn, id, pin))
    {
        cout << "Access granted. Welcome.\n\n";
    }
    else
    {
        cout << "Access denied.\n\n" << endl;
    }
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
                handleLogin(conn);
                break;
                
            case 4:
                cout << "Exiting BankCore, Goodbye.\n";
                running = false;
                break;

            default:
                cout << "Invalid option. Try again.\n\n";
                break;
        }
    }
}