// This will handle file read & write operations
#include "file_ops.h"

int readFile(std::string fileName)
{
    //  Open the text file
    std::ifstream file(fileName);

    // Error opening the file
    if (!file.is_open())
    {
        std::cerr << "Error Opening the File!!!" << std::endl;
        return 1;
    }

    // Storing contents of a line in a string then printing it
    std::string line;
    while (std::getline(file, line))
        std::cout << line << std::endl;

    file.close();
    return 0;
}

int writeFile(std::string fileName)
{

    // Create the file
    std::ofstream file(fileName);

    // Error opening the file
    if (!file.is_open())
    {
        std::cerr << "Error Opening the File!!!" << std::endl;
        return 1;
    }

    // Storing input text to linked lists for editing previous lines
    LineNode *linesHead = nullptr;
    LineNode *current = linesHead;
    int currentLine = 0;
    // Write to file
    std::string line;
    do
    {
        std::cout << ++currentLine << ": ";
        std::getline(std::cin, line);
        if (line != "/cmd" and line != "/i" and line != "/e")
        {

            if (!linesHead)
            {
                linesHead = new LineNode(line);
                current = linesHead;
            }
            else
            {
                current->nextLine = new LineNode(line);
                current = current->nextLine;
            }
        }
        else if (line == "/i")
        {
            // If insert flag triggered get line number to insert
            int lineNumber;

            std::cout << "Enter line number to insert: ";
            cleanInput(lineNumber);
            if (!linesHead)
            {
                std::cout << "Input the line:" << std::endl;
                std::cout << currentLine << ": ";
                std::getline(std::cin, line);
                linesHead = new LineNode(line);
                current = linesHead;
            }
            else
            {
                LineNode *lineFinder = linesHead;
                std::cout << "Input the line:" << std::endl;
                std::cout << currentLine << ": ";
                if (lineNumber == 1)
                {
                    // Get the new line
                    std::getline(std::cin, line);
                    LineNode *insertedLine = new LineNode(line);
                    insertedLine->nextLine = linesHead;
                    linesHead = insertedLine;
                }
                else
                {
                    for (int i = 1; i < std::max(1, lineNumber - 1); ++i)
                    {
                        if (!lineFinder->nextLine)
                        {
                            // If line number exceeds the actual amount of lines, edit last line
                            break;
                        }
                        else
                            lineFinder = lineFinder->nextLine;
                    }
                    // Get the new line
                    std::getline(std::cin, line);
                    LineNode *insertedLine = new LineNode(line);
                    insertedLine->nextLine = lineFinder->nextLine;
                    lineFinder->nextLine = insertedLine;
                }
            }
        }
        else if (line == "/cmd")
        {
            // If user wants to return to main menu, add linked list buffer to file, delete linked list
            while (linesHead)
            {
                file << linesHead->line << std::endl;

                LineNode *prev = linesHead;
                linesHead = linesHead->nextLine;
                prev->nextLine = nullptr;
                delete prev;
            }
        }
        else if (line == "/e")
        {
            // Edit a particular line
            std::cout << "Feature not implemented yet!" << std::endl;
        }

    } while (line != "/cmd");
    return 0;
}
