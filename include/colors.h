#ifndef COLORS_H
#define COLORS_H

// ANSI escape sequences for colored terminal output.
// Windows: ANSI mode is enabled by banner.cpp::enableANSI() at startup.

namespace color
{
    constexpr const char* RESET    = "\033[0m";
    constexpr const char* BOLD     = "\033[1m";
    constexpr const char* DIM      = "\033[2m";

    constexpr const char* RED      = "\033[31m";
    constexpr const char* GREEN    = "\033[32m";
    constexpr const char* YELLOW   = "\033[33m";
    constexpr const char* CYAN     = "\033[36m";
    constexpr const char* WHITE    = "\033[37m";

    constexpr const char* B_RED    = "\033[1;31m";
    constexpr const char* B_GREEN  = "\033[1;32m";
    constexpr const char* B_YELLOW = "\033[1;33m";
    constexpr const char* B_CYAN   = "\033[1;36m";
    constexpr const char* B_WHITE  = "\033[1;37m";
}

#endif
