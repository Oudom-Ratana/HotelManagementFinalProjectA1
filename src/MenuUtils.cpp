#include "../include/MenuUtils.hpp"
#include <iostream>

using namespace std;
using namespace tabulate;

void printMenuAsTable(vector<string> menuList, string menuRole)
{
    Table table;
    table.add_row({menuRole});
    for (string list : menuList)
        table.add_row({list});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void printLabel(string label)
{
    Table table;
    table.add_row({label});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void pressEnter()
{
    cout << "---------<Press Enter to continue>---------" << endl;
    cin.ignore();
    cin.get();
}