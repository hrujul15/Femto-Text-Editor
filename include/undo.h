#ifndef UNDO_H
#define UNDO_H
#include <string>
#include <vector>
struct UndoNode
{
    int lineInit;
    int lineEnd;
    std::string line;
    std::vector<std::string> lines;
    // Undo Insert
    UndoNode(int insertLineNum)
    {
        this->lineInit = insertLineNum;
        lineEnd = -1;
        line = "";
        lines.resize(0);
    }
    // Undo Edit
    UndoNode(int editedLineNum, std::string originalLine)
    {
        lineInit = editedLineNum;
        line = originalLine;
    }
    // Undo Write Line
    UndoNode(int latestLineWritten)
    {
        lineInit = latestLineWritten;
    }
    // Undo Delete Line
    UndoNode(int deleteLine, std::string originalLine)
    {
        lineInit = deleteLine;
        line = originalLine;
    }
    // Undo Multiple Line Delete
    UndoNode(int deleteLine, std::vector<std::string> originalLines)
    {
        lines = originalLines;
        line = deleteLine;
    }
    // Undo Clipboard Paste
    UndoNode(int clipLineInit, int clipLineEnd)
    {
        lineInit = clipLineInit;
        lineEnd = clipLineEnd;
    }
};
#endif