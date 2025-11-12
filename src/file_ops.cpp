// This will handle file read & write operations
#include "file_ops.h"
#include "autosave.h"
#include "undo.h"
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
        // if program is terminated unexpectedly, create autosave
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

        while (true)
        {
            std::cout << YELLOW << " Auto-save backup found for this file." << RESET << std::endl;
            std::cout << CYAN << "Do you want to restore it? (y/n): " << RESET;

            char c;
            std::cin >> c;

            // Clear input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (c == 'y' || c == 'Y')
            {
                std::string data = readAutoSave(fileName);
                std::ofstream restore(fileName);
                restore << data;
                restore.close();
                std::cout << GREEN << " Restored from auto-save.\n"
                          << RESET;
                break;
            }
            else if (c == 'n' || c == 'N')
            {
                deleteAutoSave(fileName);
                std::cout << MAGENTA << " Auto-save deleted." << RESET << std::endl;
                break;
            }
            else
            {
                std::cout << RED << " Invalid choice! Please enter y or n." << RESET << std::endl;
            }
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
    // Creating stack for undo ops
    std::stack<UndoNode *> undoStack;
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

            while (true)
            {
                std::cout << YELLOW << " Auto-save backup found for this file." << RESET << std::endl;
                std::cout << CYAN << "Do you want to restore it? (y/n): " << RESET;

                char c;
                std::cin >> c;

                // Clear input buffer
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (c == 'y' || c == 'Y')
                {
                    std::string data = readAutoSave(fileName);
                    std::ofstream restore(fileName);
                    restore << data;
                    restore.close();
                    std::cout << GREEN << " Restored from auto-save.\n"
                              << RESET;
                    break;
                }
                else if (c == 'n' || c == 'N')
                {
                    deleteAutoSave(fileName);
                    std::cout << MAGENTA << " Auto-save deleted." << RESET << std::endl;
                    break;
                }
                else
                {
                    std::cout << RED << " Invalid choice! Please enter y or n." << RESET << std::endl;
                }
            }
        }
        std::cout << CYAN
                  << "Type\n"
                  << "/cmd: to return to main menu\n"
                  << "/e:   to edit a particular line\n"
                  << "/i:   to insert a line at a particular line number\n"
                  << "/d:   to delete a range of lines\n"
                  << "/D:   to delete a single line\n"
                  << "/u:   to undo the last operation"
                  << RESET << std::endl;
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
        if (line != "/cmd" and line != "/i" and line != "/e" and line != "/d" and line != "/D" and line != "/u")
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
            UndoNode *node = new UndoNode('w', currentLine);
            undoStack.push(node);
            createAutoSave(fileName, getAllFileContent(linesHead));
        }
        else if (line == "/u")
        {
            // Implemnting undo

            if (!undoStack.empty())
            {

                UndoNode *operation = undoStack.top();
                // Perform necessary undo operation
                performUndo(operation, linesHead, current);
                undoStack.pop();
                currentLine = traverseAndPrint(linesHead);
            }
            else
            {
                std::cout << RED << "Nothing to Undo!" << RESET << std::endl;
                --currentLine;
            }
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
                    std::string deleteString = deletedLine->line;
                    linesHead = linesHead->nextLine;
                    delete deletedLine;
                    --currentLine;
                    current = nullptr;
                    UndoNode *node = new UndoNode('D', 1, deleteString);
                    undoStack.push(node);
                }
                else
                {
                    std::string deleteString;
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
                        // Pushing the deleted string to stack
                        deleteString = lineFinder->line;
                        UndoNode *node = new UndoNode('D', lineNumber, deleteString);
                        undoStack.push(node);
                        // Deleting line from linked list
                        delete lineFinder;
                    }
                    else
                    {
                        // Pushing the deleted string to stack
                        deleteString = deletedLine->line;
                        UndoNode *node = new UndoNode('D', lineNumber, deleteString);
                        undoStack.push(node);
                        lineFinder->nextLine = deletedLine->nextLine;
                        // Deleteing line from linked list
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
                std::vector<std::string> deletedLines;
                // Check if input extraction was successful
                if (!(std::cin >> startLine >> endLine))
                {
                    std::cout << RED << "Invalid input! Please enter numbers only." << RESET << std::endl;
                    std::cin.clear();                                                   // Clear the error flags
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
                    currentLine = traverseAndPrint(linesHead);
                    current = nullptr;
                    continue;
                }

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer

                if (startLine > endLine)
                    std::swap(startLine, endLine);
                if (startLine < 1)
                    startLine = 1;

                LineNode *temp = linesHead;
                LineNode *prev = nullptr;
                int lineNum = 1;

                // Move to the start line
                while (temp && lineNum < startLine)
                {
                    prev = temp;
                    temp = temp->nextLine;
                    ++lineNum;
                }

                // Nothing to delete
                if (!temp)
                {
                    std::cout << RED << "Invalid range!" << RESET << std::endl;
                    currentLine = traverseAndPrint(linesHead);
                    createAutoSave(fileName, getAllFileContent(linesHead));
                    current = nullptr;
                    continue;
                }

                // Delete from startLine to endLine
                LineNode *afterEnd = temp;
                while (afterEnd && lineNum <= endLine)
                {
                    LineNode *toDelete = afterEnd;
                    // Puttting deleted line string to vector to undo
                    deletedLines.push_back(toDelete->line);
                    afterEnd = afterEnd->nextLine;
                    delete toDelete;
                    ++lineNum;
                }

                // Reconnect linked list
                if (prev)
                    prev->nextLine = afterEnd;
                else
                    linesHead = afterEnd;

                std::cout << GREEN << "Deleted lines from " << startLine << " to " << endLine << "." << RESET << std::endl;
                // Put start end line and vector to undo stack
                UndoNode *node = new UndoNode('d', startLine, "", endLine, deletedLines);
                undoStack.push(node);
                // Reprint updated file
                currentLine = traverseAndPrint(linesHead);
                g_activeHead = linesHead;
                createAutoSave(fileName, getAllFileContent(g_activeHead));
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
                // Pushing linenum to undostack
                UndoNode *node = new UndoNode('i', lineNumber);
                undoStack.push(node);
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

                // Pushing linenum to undostack
                UndoNode *node = new UndoNode('i', lineNumber);
                undoStack.push(node);

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
                // Pushing linenum to undostack
                UndoNode *node = new UndoNode('e', lineNumber, "");
                undoStack.push(node);
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
                    // Pushing linenum to undostack
                    UndoNode *node = new UndoNode('e', lineNumber, linesHead->line);
                    undoStack.push(node);
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
                    // Pushing linenum to undostack
                    UndoNode *node = new UndoNode('e', lineNumber, lineFinder->line);
                    undoStack.push(node);
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
        std::cout << GREY << ++travLine << ": " << RESET;
        std::cout << trav->line << std::endl;
        trav = trav->nextLine;
    }

    return travLine;
}
