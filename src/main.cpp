#include <iostream>
#include "banner.h"
#include "database.h"
#include "menu.h"
#include "actions.h"   // ← ADD THIS LINE

using namespace std;

int main()
{
    showStartup();

    MYSQL* conn = connectDatabase();
    if (conn == nullptr)
    {
        return 1;
    }

    cout << endl;
    runMenu(conn);

    disconnectDatabase(conn);
    return 0;
}