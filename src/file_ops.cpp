// This will handle file read & write operations
#include "file_ops.h"
#include "autosave.h"
#include <csignal>

// Global variables to track active file for autosave
std::string g_activeFile = "";
LineNode *g_activeHead = nullptr;

// Function to get entire file content from linked list
std::string getAllFileContent(LineNode *head)
{
    std::string content;
    LineNode *temp = head;

    while (temp)
    {
        content += temp->line;

        //  Always add newline **except** when it’s the last line
        if (temp->nextLine)
            content += "\n";

        temp = temp->nextLine;
    }

    return content;
}

// Signal handler for unexpected termination
void handleExitSignal(int signal)
{
    if (!g_activeFile.empty() && g_activeHead)
    {
        std::cout << "\n\n[AutoSave] Program closed unexpectedly. Creating backup...\n";
        createAutoSave(g_activeFile, getAllFileContent(g_activeHead));
    }
    std::exit(signal);
}

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
    // Check for auto-save backup
    if (checkAutoSave(fileName))
    {
        std::cout << " Auto-save backup found for this file.\n";
        std::cout << "Do you want to restore it? (y/n): ";
        char c;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 'y' || c == 'Y')
        {
            std::string data = readAutoSave(fileName);
            std::ofstream restore(fileName);
            restore << data;
            restore.close();
            std::cout << GREEN << "Restored from auto-save.\n"
                      << RESET;
        }
        else
        {
            deleteAutoSave(fileName);
            std::cout << "Auto-save deleted.\n";
        }
    }

    std::signal(SIGINT, handleExitSignal);
    std::signal(SIGTERM, handleExitSignal);

    g_activeFile = fileName;

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

    // Setup signal handlers for autosave on unexpected exit
    std::signal(SIGINT, handleExitSignal);
    std::signal(SIGTERM, handleExitSignal);

    g_activeFile = fileName;
    g_activeHead = nullptr; // will point to linesHead once loaded

    // Storing input text to linked lists for editing previous lines
    LineNode *linesHead = nullptr;
    LineNode *current = linesHead;
    // Create the file or open in case editing a pre-existing file
    std::ios_base::openmode mode;
    int currentLine = 0;
    if (editing)
    {
        // Check for auto-save backup
        if (checkAutoSave(fileName))
        {
            std::cout << YELLOW << "\nAuto-save backup found for this file.\n";
            std::cout << "Do you want to restore it? (y/n): " << RESET;
            char c;
            std::cin >> c;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (c == 'y' || c == 'Y')
            {
                std::string data = readAutoSave(fileName);
                std::ofstream restore(fileName);
                restore << data;
                restore.close();
                std::cout << GREEN << "Restored from auto-save.\n"
                          << RESET;
            }
            else
            {
                deleteAutoSave(fileName);
                std::cout << "Auto-save deleted.\n";
            }
        }

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
        // Set global head for autosave
        g_activeHead = linesHead; // set global head for autosave
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
        if (line != "/cmd" and line != "/i" and line != "/e" and line != "/d" and line != "/D")
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
            // Update global head for autosave
            createAutoSave(fileName, getAllFileContent(linesHead));
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
        else if (line == "/D")
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
                // Update global head for autosave
                createAutoSave(fileName, getAllFileContent(linesHead));

                current = nullptr;
            }
        }
        // delete a range of lines
        else if (line == "/d")
        {
            if (!linesHead)
            {
                std::cout << RED << "Nothing to delete!" << RESET << std::endl;
                --currentLine;
            }
            else
            {
                int startLine, endLine;
                std::cout << GREEN << "Enter starting and ending line numbers to delete (e.g., 3 8): " << RESET;

                // Read both numbers on the same line
                std::cin >> startLine >> endLine;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer

                if (startLine > endLine)
                    std::swap(startLine, endLine);
                if (startLine < 1)
                    startLine = 1;

                LineNode *temp = linesHead;
                LineNode *prev = nullptr;
                int lineNum = 1;

                while (temp && lineNum < startLine)
                {
                    prev = temp;
                    temp = temp->nextLine;
                    ++lineNum;
                }

                if (!temp)
                {
                    std::cout << RED << "Invalid range " << RESET << std::endl;
                    currentLine = traverseAndPrint(linesHead);
                    // Update global head for autosave
                    createAutoSave(fileName, getAllFileContent(linesHead));
                    current = nullptr;
                    continue;
                }

                // Delete between startLine and endLine
                LineNode *afterEnd = temp;
                while (afterEnd and lineNum <= endLine)
                {
                    LineNode *toDelete = afterEnd;
                    afterEnd = afterEnd->nextLine;
                    delete toDelete;
                    ++lineNum;
                }

                if (prev)
                    prev->nextLine = afterEnd;
                else
                    linesHead = afterEnd;

                std::cout << GREEN << "Deleted lines from " << startLine << " to " << endLine << "." << RESET << std::endl;

                // Reprint updated file
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
                // Update global head for autosave
                createAutoSave(fileName, getAllFileContent(linesHead));
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
            // Delete autosave on normal exit
            deleteAutoSave(fileName);
            // Reset global autosave trackers
            g_activeFile = "";
            g_activeHead = nullptr;
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
                // Update global head for autosave
                createAutoSave(fileName, getAllFileContent(linesHead));
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