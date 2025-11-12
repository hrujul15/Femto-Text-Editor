#include "undo.h"
void performUndo(UndoNode *operation, LineNode *&head, LineNode *&current)
{
    char state = operation->state;
    // for undo insert, delete that line
    if (state == 'i')
        undoInsert(operation, head, current);
    // for undo edit, edit that line to its orignal
    else if (state == 'e')
        undoEdit(operation, head, current);
    // for undo writing line, delete last line!
    else if (state == 'w')
        undoWrite(operation, head, current);
    // for undo delete single line, insert the line at deleted positon!
    else if (state == 'D')
        undoDelete(operation, head, current);
    // for undo delete multiple lines, insert lines from the given starting index
    else if (state == 'd')
        undoDeleteMul(operation, head, current);
    std::cout << GREEN << "UNDO Success" << RESET << std::endl;
}
void undoDeleteMul(UndoNode *node, LineNode *&linesHead, LineNode *&current)
{
    int startLine = node->lineInit, endLine = node->lineEnd;
    std::vector<std::string> linesPutBack = node->lines;
    // if we deleted the entire list, remake from start!
    if (!linesHead)
    {
        linesHead = new LineNode(linesPutBack.front());
        current = linesHead;
        int size = linesPutBack.size();
        for (int i = 1; i < size; ++i)
        {
            current->nextLine = new LineNode(linesPutBack[i]);
            current = current->nextLine;
        }
        return;
    }
    if (startLine > endLine)
        std::swap(startLine, endLine);
    if (startLine < 1)
        startLine = 1;

    LineNode *temp = linesHead;
    LineNode *prev = nullptr;
    int lineNum = 1;
    // Move to the start line
    while (temp && lineNum < startLine - 1)
    {

        prev = temp;
        temp = temp->nextLine;
        ++lineNum;
    }

    // Delete from startLine to endLine

    LineNode *startingInsert = temp;

    // pointer to line we will start inserting from, ie start from 3, so this ptr is at 3rd node, wont work if the line itself is deleted
    if (startLine == 1)
    {

        LineNode *ogHead = linesHead;

        linesHead = new LineNode(linesPutBack.front());
        current = linesHead;
        int size = linesPutBack.size();
        for (int i = 1; i < size; ++i)
        {
            current->nextLine = new LineNode(linesPutBack[i]);
            current = current->nextLine;
        }
        current->nextLine = ogHead;
        current = nullptr;
        return;
    }

    // Below assumes we deleted from line 2 to line k,

    if (!startingInsert)
    {
        std::cout << RED << "Failed to UNDO" << RESET << std::endl;
        return;
    }

    LineNode *lastLine = startingInsert->nextLine;

    for (auto it : linesPutBack)
    {
        startingInsert->nextLine = new LineNode(it);
        startingInsert = startingInsert->nextLine;
    }
    // reconnecting the linked list after inserting new nodes
    startingInsert->nextLine = lastLine;

    current = nullptr;

    return;
}
void undoDelete(UndoNode *node, LineNode *&linesHead, LineNode *&current)
{
    // get line number to insert
    int lineNumber = node->lineInit;
    std::string line = node->line;
    LineNode *lineFinder = linesHead;
    if (lineNumber <= 1)
    {
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
        LineNode *insertedLine = new LineNode(line);
        insertedLine->nextLine = lineFinder->nextLine;
        lineFinder->nextLine = insertedLine;
    }
}
void undoWrite(UndoNode *node, LineNode *&linesHead, LineNode *&current)
{
    // Same logic as undo insert but simply delete last line!
    undoInsert(node, linesHead, current);
}
void undoEdit(UndoNode *node, LineNode *&linesHead, LineNode *&current)
{
    int lineNumber = node->lineInit;
    std::string line = node->line;
    int currentLine = 0;
    LineNode *trav = linesHead;
    while (trav)
    {
        currentLine++;
        trav = trav->nextLine;
    }

    LineNode *lineFinder = linesHead;
    if (lineNumber <= 1)
    {
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

        lineFinder->line = line;
    }
}
void undoInsert(UndoNode *node, LineNode *&linesHead, LineNode *&current)
{
    int lineNumber = node->lineInit;
    int maxLines = 0;
    LineNode *trav = linesHead;
    while (trav)
    {
        maxLines++;
        trav = trav->nextLine;
    }
    // Although not neccessary at all, kept it as an extra safety check
    if (!linesHead)
    {
        std::cout << RED << "Nothing to delete!" << RESET << std::endl;
    }
    else
    {

        LineNode *lineFinder = linesHead;
        if (maxLines <= 2 or lineNumber <= 2)
        {
            // Case 1: exactly 2 lines
            if (std::min(maxLines, lineNumber) == 2)
            {
                LineNode *secondLine = linesHead->nextLine;
                linesHead->nextLine = secondLine->nextLine;
                secondLine->nextLine = nullptr;
                delete secondLine;
            }
            // Case 2: less than 2 lines ie 1 line only
            else
            {

                LineNode *oldHead = linesHead;

                linesHead = (linesHead)->nextLine;

                delete oldHead;
            }
            return;
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

                // Deleting line from linked list
                delete lineFinder;
            }
            else
            {

                lineFinder->nextLine = deletedLine->nextLine;
                // Deleteing line from linked list
                delete deletedLine;
            }
        }
        current = nullptr;
    }
}