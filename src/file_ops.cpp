// This will handle file read & write operations
#include "file_ops.h"

int readFile(std::string fileName)
{
    //  Open the text file
    std::ifstream file(fileName);

    // Error opening the file
    if (!file.is_open())
    {
        std::cerr << RED << "Error Opening the File!!!" << RESET << std::endl;
        return 1;
    }

    // Storing contents of a line in a string then printing it
    std::string line;
    while (std::getline(file, line))
        std::cout << line << std::endl;

    file.close();
    std::cout << RESET;
    return 0;
}
int editFile(std::string fileName)
{
    // If editing reuse writefile function, and instead append at end
    writeFile(fileName, true);
    std::cout << RESET;
    return 0;
}
int writeFile(std::string fileName, bool editing)
{
    // Creating stack for undo-redo ops
    std::stack<LineNode *> Undo;
    std::stack<LineNode *> Redo;

    // Storing input text to linked lists for editing previous lines
    LineNode *linesHead = nullptr;
    LineNode *current = linesHead;
    // Create the file or open in case editing a pre-existing file
    std::ios_base::openmode mode;
    int currentLine = 0;
    if (editing)
    {
        // store the pre-existing lines in linked list
        //  Open the text file
        std::ifstream file(fileName);
        // Error opening the file
        if (!file.is_open())
        {
            std::cerr << RED << "Error Opening the File!!!" << RESET << std::endl;
            return 1;
        }
        // Storing contents of a line in a string then printing it
        // and adding to linked list as well
        std::string line;
        while (std::getline(file, line))
        {
            std::cout << ++currentLine << ": ";
            std::cout << line << std::endl;

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
        file.close();
        mode = std::ios::app;
    }
    else
        mode = std::ios::out;
    std::fstream file(fileName, mode);

    // Error opening the file
    if (!file.is_open())
    {
        std::cerr << RED << "Error Opening the File!!!" << RESET << std::endl;
        return 1;
    }

    // Write to file
    std::string line;
    do
    {
        std::cout << ++currentLine << ": ";
        std::getline(std::cin, line);
        if (line != "/cmd" and line != "/i" and line != "/e" and line != "/d")
        {

            if (!linesHead)
            {
                linesHead = new LineNode(line);
                current = linesHead;
            }
            else
            {
                // currentLine = 1;
                if (!current)
                {
                    current = linesHead;
                    while (current->nextLine)
                    {
                        current = current->nextLine;
                        // ++currentLine;
                    }
                }
                current->nextLine = new LineNode(line);
                current = current->nextLine;
            }
        }
        else if (line == "/u")
        {
            // Implemnting undo
            std::cout << "Unimplemented feature!" << std::endl;
        }
        else if (line == "/r")
        {
            std::cout << "Unimplemented feature!" << std::endl;
        }
        else if (line == "/d")
        {
            // If delete flag triggered get line number to delete
            int lineNumber;

            if (!linesHead)
            {
                std::cout << RED << "Nothing to delete!" << RESET << std::endl;
                --currentLine;
            }
            else
            {
                std::cout << GREEN << "Enter line number to delete: " << RESET;
                cleanInput(lineNumber);
                LineNode *lineFinder = linesHead;
                if (lineNumber <= 1 or currentLine <= 2)
                {
                    // Delete Line
                    LineNode *deletedLine = linesHead;
                    linesHead = linesHead->nextLine;
                    delete deletedLine;
                    --currentLine;
                    current = nullptr;
                }
                else
                {
                    for (int i = 1; i < std::max(1, lineNumber - 1); ++i)
                    {
                        if (!lineFinder->nextLine)
                        {
                            // If line number exceeds the actual amount of lines, edit last line
                            // ++currentLine;
                            break;
                        }
                        else
                            lineFinder = lineFinder->nextLine;
                    }
                    // Delete line
                    LineNode *deletedLine = lineFinder->nextLine;
                    // If we deleting last line then deletedLine == nullptr
                    if (!deletedLine)
                    {
                        // if linefinder = last node deleting it will cause problems, dangling ptr
                        // so going to second last and then deleting
                        // but if there is one node then even this can cause problems
                        // so this base case "if (lineNumber <= 1 or currentLine <= 2)" at start
                        LineNode *starter = linesHead;
                        while (starter->nextLine != lineFinder)
                        {
                            starter = starter->nextLine;
                        }
                        starter->nextLine = nullptr;
                        delete lineFinder;
                    }
                    else
                    {
                        lineFinder->nextLine = deletedLine->nextLine;
                        delete deletedLine;
                    }
                    --currentLine;
                }

                // Reprint contents of file
                --currentLine;
                currentLine = traverseAndPrint(linesHead);
                current = nullptr;
            }
        }
        else if (line == "/i")
        {
            // If insert flag triggered get line number to insert
            int lineNumber;

            std::cout << GREEN << "Enter line number to insert: " << RESET;
            cleanInput(lineNumber);
            if (!linesHead)
            {
                std::cout << GREEN << "Input the line:" << RESET << std::endl;
                std::cout << std::max(1, std::min(lineNumber, currentLine)) << ": ";
                std::getline(std::cin, line);
                linesHead = new LineNode(line);
                current = linesHead;
                currentLine = traverseAndPrint(linesHead);
                // current = nullptr;
            }
            else
            {
                LineNode *lineFinder = linesHead;
                std::cout << GREEN << "Input the line:" << RESET << std::endl;
                std::cout << std::max(1, std::min(lineNumber, currentLine)) << ": ";
                if (lineNumber <= 1)
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
                // Reprint contents
                currentLine = traverseAndPrint(linesHead);
                // current = nullptr;
            }
        }
        else if (line == "/cmd")
        {
            file.close(); // Close previous stream
            file.open(fileName, std::ios::out);
            // If user wants to return to main menu, add linked list buffer to file, delete linked list
            LineNode *root = linesHead;
            while (linesHead)
            {
                file << linesHead->line << std::endl;
                linesHead = linesHead->nextLine;
            }
            while (root)
            {
                LineNode *prev = root;
                root = root->nextLine;
                delete prev;
            }
            linesHead = nullptr;
            current = nullptr;

            file.close();
        }
        else if (line == "/e")
        {
            // Edit a particular line
            // If edit flag triggered get line number to edit
            int lineNumber;

            std::cout << GREEN << "Enter line number to edit: " << RESET;
            cleanInput(lineNumber);
            if (!linesHead)
            {
                std::cout << GREEN << "Input the line:" << RESET << std::endl;
                std::cout << std::max(1, std::min(lineNumber, currentLine)) << ": ";
                std::getline(std::cin, line);
                linesHead = new LineNode(line);
                current = linesHead;
                currentLine = traverseAndPrint(linesHead);
            }
            else
            {
                LineNode *lineFinder = linesHead;
                std::cout << GREEN << "Input the line:" << RESET << std::endl;
                std::cout << std::max(1, std::min(lineNumber, currentLine - 1)) << ": ";
                if (lineNumber <= 1)
                {
                    // Get the new line
                    std::getline(std::cin, line);
                    linesHead->line = line;
                }
                else
                {
                    for (int i = 1; i < std::max(1, lineNumber); ++i)
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
                    lineFinder->line = line;
                }
                currentLine = traverseAndPrint(linesHead);
                // current = nullptr;
            }
        }

    } while (line != "/cmd");
    std::cout << RESET;
    return 0;
}
int traverseAndPrint(LineNode *trav)
{
    int travLine = 0;
    std::cout << std::endl;
    while (trav)
    {
        std::cout << ++travLine << ": ";
        std::cout << trav->line << std::endl;
        trav = trav->nextLine;
    }

    return travLine;
}