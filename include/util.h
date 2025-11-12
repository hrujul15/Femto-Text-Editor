#ifndef UTIL_H
#define UTIL_H
#include <limits>
#include <iostream>
// Colored text for prettty print
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define GREY "\033[2;37m"
#define WHITE "\033[37m"
struct LineNode
{
    std::string line;
    LineNode *nextLine = nullptr;
    LineNode(std::string line)
    {
        this->line = line;
        nextLine = nullptr;
    }
};
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
            std::cout << RED << "Invalid input! Please enter again: " << RESET;
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    std::cout << RESET;
}
// CLearing input buffer and protection against invalid inputs
#endif