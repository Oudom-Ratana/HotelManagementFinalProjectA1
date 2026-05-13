#ifndef MENUUTILS_HPP
#define MENUUTILS_HPP

#include <string>
#include <vector>
#include <tabulate/table.hpp>

void printMenuAsTable(std::vector<std::string> menuList, std::string menuRole);
void printLabel(std::string label);
void pressEnter();

#endif