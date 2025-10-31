#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <iostream>
#define cleanInput(input) \
    std::cin >> input;    \
    std::cin.ignore(); // To clear input buffer
void welcome();
void chooseOperation(int option);
#endif