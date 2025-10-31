// All the menu functions
// From here we will select which operation to perform

#include "main_menu.h"
#include "file_ops.h"
void welcome()
{
    int option;
    std::cout << "Welcome to Femto-Text-Editor!" << std::endl;
    std::cout << "Please choose an operation to perform:" << std::endl;
    std::cout << "-1: Exit" << std::endl;
    std::cout << "0: Read a file" << std::endl;
    std::cout << "1: Create a file and edit it" << std::endl;
    cleanInput(option);
    chooseOperation(option);
}
void chooseOperation(int option)
{
    switch (option)
    {
    case -1:
    {
        return;
    }
    case 0:
    {
        std::string fileName;
        std::cout << "Type the path to the file to open: " << std::endl;
        std::cin >> fileName;
        std::cin.ignore(); // To clear input buffer
        readFile(fileName);
        // Do more operations
        std::cout << "Please choose an operation to perform: ";
        cleanInput(option);
        chooseOperation(option);
    }
    case 1:
    {
        std::string fileName;
        std::cout << "Type the path to the file to create and edit: " << std::endl;
        std::cin >> fileName;
        std::cin.ignore(); // To clear input buffer
        writeFile(fileName);
        // Do more operations
        std::cout << "Please choose an operation to perform: ";
        cleanInput(option);
        chooseOperation(option);
    }
    default:
    {
        std::cout << "Invalid option selected, reselect!" << std::endl;
        std::cout << "Please choose an operation to perform: ";
        cleanInput(option);
        chooseOperation(option);
    }
    }
}
