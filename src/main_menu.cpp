// All the menu functions
// From here we will select which operation to perform

#include "main_menu.h"
#include "file_ops.h"
#include "clipboard.h"
#include "search.h"
#include "stats.h"

void welcome()
{
    int option;
    std::cout << MAGENTA << "Welcome to Femto-Text-Editor!" << RESET << std::endl;
    std::cout << YELLOW << "[Tip] Your last line is only saved after pressing Enter!" << RESET << std::endl;
    std::cout << BLUE << "Please choose an operation to perform:" << std::endl;
    std::cout << "-1: Exit" << std::endl;
    std::cout << "0: Read a file" << std::endl;
    std::cout << "1: Create a file and edit it" << std::endl;
    std::cout << "2: Open a file and edit it" << std::endl;
    std::cout << "3: Copy text from file" << std::endl;
    std::cout << "4: Paste text into file" << std::endl;
    std::cout << "5: Search in file" << std::endl;
    std::cout << "6: Display File Statistics" << std::endl;////////
    
    std::cout << "7: Find and Replace in File" <<  std::endl;
    std::cout << "8: Print this Command Pallete" << RESET << std::endl;////////

    cleanInput(option);
    chooseOperation(option);
}
void chooseOperation(int option)
{
    while (true)
    {
        switch (option)
        {
        case -1:
        {
            return;
            break;
        }
        case 0:
        {
            std::string fileName;
            std::cout << CYAN << "Type the path to the file to open: " << RESET << std::endl;
            std::cin >> fileName;
            std::cin.ignore(); // To clear input buffer
            readFile(fileName);
            // Do more operations
            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
        case 1:
        {
            std::string fileName;
            std::cout << CYAN << "Type the path to the file to create and edit: " << RESET << std::endl;
            std::cin >> fileName;
            std::cin.ignore(); // To clear input buffer
           std::cout << CYAN << "Type /cmd to return to main menu\n/e edit a particular line\n/i to insert a line at particular line number\n/d to delete a range of line\n/D to delete a single line" << RESET << std::endl;
            writeFile(fileName);
            // Do more operations
            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
        case 2:
        {
            std::string fileName;
            std::cout << CYAN << "Type the path to the file to open and edit: " << RESET << std::endl;
            std::cin >> fileName;
            std::cin.ignore(); // To clear input buffer
            std::cout << CYAN << "Type /cmd to return to main menu\n/e edit a particular line\n/i to insert a line at particular line number\n/d to delete a range of line\n/D to delete a single line" << RESET << std::endl;
            editFile(fileName);
            // Do more operations
            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
   case 3:
        {
            std::string mode;
            std::cout << CYAN << "Enter mode (/c = copy, /p = paste): " << RESET;
            std::cin >> mode;

            if (mode == "/c") {
                                std::string fileName;
                                int startLine, endLine;
                                std::cout << CYAN << "Enter file name to copy from: " << RESET;
                                std::cin >> fileName;

                                std::ifstream fileCheck(fileName);
                                 if (!fileCheck.is_open()) {
                                 std::cout << RED << "❌ Error: File '" << fileName << "' not found or cannot be opened!" << RESET << std::endl;
                                 } 
                                 else {
                                std::cout << CYAN << "Enter start and end line numbers to copy: " << RESET;
                                std::cin >> startLine >> endLine;
                                copyFromFile(fileName, startLine, endLine);
                                }

                                fileCheck.close();
                              } 
                else if (mode == "/p") {
                                std::string fileName;
                                int lineNumber;
                                std::cout << CYAN << "Enter file name to paste into: " << RESET;
                                std::cin >> fileName;

                                std::ifstream fileCheck(fileName);
                                if (!fileCheck.is_open()) {
                                 std::cout << RED << "❌ Error: File '" << fileName << "' not found or cannot be opened!" << RESET << std::endl;
                                } 
                                else {
                                std::cout << CYAN << "Enter line number where to paste: " << RESET;
                                std::cin >> lineNumber;
                                pasteToFile(fileName, lineNumber);
                            }

                                 fileCheck.close();
                                      } 
                 else {
                                 std::cout << RED << "Invalid mode. Use /c for copy or /p for paste." << RESET << std::endl;
                 }

                 std::cout << CYAN << "Please choose an operation to perform: " << RESET;
                cleanInput(option);
                break;
        }

        case 4:
        {
            std::string fileName, mode;
            std::cout << CYAN << "Enter file name to search: " << RESET;
            std::cin >> fileName;
            std::cout << CYAN << "Choose search mode (/f = find word, /m = most frequent word): " << RESET;
            std::cin >> mode;

            if (mode == "/f")
            {
                std::string word;
                std::cout << CYAN << "Enter word to find: " << RESET;
                std::cin >> word;
                findWordInFile(fileName, word);
            }
            else if (mode == "/m")
            {
                findMostFrequentWord(fileName);
            }
            else
            {
                std::cout << RED << "Invalid mode. Use /f or /m." << RESET << std::endl;
            }

            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
        case 6:
        {
            std::string fileName;
            std::cout << CYAN << "Enter file name to search: " << RESET;
            std::cin >> fileName;
            std::cout << CYAN << "Here are the Text Statistics:" << RESET << std::endl;
            
            countStats(fileName);
            
            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
        case 7:
        {
            std::string fileName, target, replacement;
            std::cout << CYAN << "Enter file name to search: " << RESET;
            std::cin >> fileName;
            std::cout << CYAN << "Enter word to find (case-sensitive): " << RESET;
            std::cin >> target;
            std::cout << CYAN << "Enter replacement word: " << RESET;
            std::cin >> replacement;

            findAndReplaceInFile(fileName, target, replacement);

            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
        case 8:
        {
        std::cout << MAGENTA << "Welcome to Femto-Text-Editor!" << RESET << std::endl;
        std::cout << BLUE << "Please choose an operation to perform:" << std::endl;
        std::cout << "-1: Exit" << std::endl;
        std::cout << "0: Read a file" << std::endl;
        std::cout << "1: Create a file and edit it" << std::endl;
        std::cout << "2: Open a file and edit it" << std::endl;
        std::cout << "3: Copy text from file" << std::endl;
        std::cout << "4: Paste text into file" << std::endl;
        std::cout << "5: Search in file" << std::endl;
        std::cout << "6: Display File Statistics" << std::endl;
        std::cout << "7: Find and Replace in File" <<  std::endl;
        std::cout << "8: Print this Command Pallete" << RESET << std::endl;
        cleanInput(option);
        break;
        }
        default:
        {
            std::cout << RED << "Invalid option selected, reselect!" << RESET << std::endl;
            std::cout << CYAN << "Please choose an operation to perform: " << RESET;
            cleanInput(option);
            break;
        }
        }
    }
}
