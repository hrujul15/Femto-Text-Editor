#ifndef UTIL_H
#define UTIL_H
#include <limits>
template <typename T>
void cleanInput(T &input)
{
    while (true)
    {
        std::cin >> input;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter again: ";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
}
// CLearing input buffer and protection against invalid inputs
#endif