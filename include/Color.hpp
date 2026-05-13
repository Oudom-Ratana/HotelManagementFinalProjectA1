#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"

inline void enableAnsiColors()
{
#ifdef _WIN32

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    SetConsoleOutputCP(CP_UTF8);
#endif
}

inline void printColored(const std::string &text, const std::string &color)
{
    std::cout << color << text << RESET;
}

inline void printSuccess(const std::string &msg)
{
    std::cout << GREEN << "[✓] " << msg << RESET << std::endl;
}

inline void printError(const std::string &msg)
{
    std::cout << RED << "[✗] " << msg << RESET << std::endl;
}

inline void printInfo(const std::string &msg)
{
    std::cout << CYAN << "[i] " << msg << RESET << std::endl;
}

inline void printWarning(const std::string &msg)
{
    std::cout << YELLOW << "[!] " << msg << RESET << std::endl;
}

inline void printBanner()
{
    std::cout << BOLD << BLUE;
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║       ROMDOUL HOTEL MANAGEMENT SYSTEM    ║\n";
    std::cout << "║        " << YELLOW << "Welcome! " << BLUE << "                       ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n"
              << RESET << std::endl;
}

#endif