#include "banner.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

static const string BANNER = R"(    ____              __            ____   __  ___                     
   / __ )____ _____  / /__   ____  / __/  /  |/  /_  ______  ____  ____
  / __  / __ `/ __ \/ //_/  / __ \/ /_   / /|_/ / / / / __ \/ __ \/_  /
 / /_/ / /_/ / / / / ,<    / /_/ / __/  / /  / / /_/ / / / / /_/ / / /_
/_____/\__,_/_/ /_/_/|_|   \____/_/    /_/  /_/\__,_/_/ /_/\____/ /___/
                                                                       )";

static string rgbColor(int r, int g, int b)
{
    return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
}

static void enableANSI()
{
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode))
    {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    SetConsoleOutputCP(CP_UTF8);
#endif
}

static void printRainbowBanner(const string& banner, double shift)
{
    int index = 0;

    for (char ch : banner)
    {
        if (ch == '\n')
        {
            cout << '\n';
            continue;
        }

        if (ch == ' ')
        {
            cout << ' ';
            index++;
            continue;
        }

        int r = static_cast<int>(127 * (sin(0.15 * index + shift) + 1));
        int g = static_cast<int>(127 * (sin(0.15 * index + shift + 2.094) + 1));
        int b = static_cast<int>(127 * (sin(0.15 * index + shift + 4.188) + 1));

        cout << rgbColor(r, g, b) << ch;
        index++;
    }

    cout << "\033[0m";
}

void showStartup()
{
    enableANSI();
    cout << "\033[2J\033[H";

    double shift = 0.0;

    for (int frame = 0; frame < 30; frame++)
    {
        cout << "\033[H";
        printRainbowBanner(BANNER, shift);
        cout.flush();

        shift += 0.3;
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    cout << "\033[H";
    printRainbowBanner(BANNER, shift);
    cout << "\n\n";
}