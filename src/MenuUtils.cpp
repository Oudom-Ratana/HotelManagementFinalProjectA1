#include "../include/MenuUtils.hpp"
#include "../include/Color.hpp"
#include <iostream>

using namespace std;
using namespace tabulate;

void printMenuAsTable(vector<string> menuList, string menuRole)
{
    Table table;
    string coloredRole = string(BOLD) + string(CYAN) + menuRole + string(RESET);
    table.add_row({coloredRole});
    for (string list : menuList)
        table.add_row({string(YELLOW) + list + RESET});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void printLabel(string label)
{
    Table table;
    string coloredLabel = string(BOLD) + MAGENTA + label + RESET;
    table.add_row({coloredLabel});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void pressEnter()
{
    printColored("---------<Press Enter to continue>---------", CYAN);
    cin.ignore();
    cin.get();
}