#ifndef UNDO_H
#define UNDO_H
#include <string>
#include <vector>
#include <stack>
#include "util.h"
struct UndoNode
{
    int lineInit;
    int lineEnd;
    char state;
    std::string line;
    std::vector<std::string> lines;
    UndoNode(char State, int lineInit, std::string LINE = "", int lineEnd = -1, std::vector<std::string> LINES = {})
    {
        this->state = State;
        if (state == 'i')
            UndoInsert(lineInit);
        else if (state == 'e')
            UndoEdit(lineInit, LINE);
        else if (state == 'w')
            UndoWrite(lineInit);
        else if (state == 'D')
            UndoDelete(lineInit, LINE);
        else if (state == 'd')
            UndoDeleteMul(lineInit, lineEnd, LINES);
        else
            lineInit = -1;
    }
    // Undo Insert
    void UndoInsert(int insertLineNum)
    {
        this->lineInit = insertLineNum;
        lineEnd = -1;
        line = "";
        lines.resize(0);
    }
    // Undo Edit
    void UndoEdit(int editedLineNum, std::string originalLine)
    {
        lineInit = editedLineNum;
        line = originalLine;
    }
    // Undo Write Line
    void UndoWrite(int latestLineWritten)
    {
        lineInit = latestLineWritten;
    }
    // Undo Delete Line
    void UndoDelete(int deleteLine, std::string originalLine)
    {
        lineInit = deleteLine;
        line = originalLine;
    }
    // Undo Multiple Line Delete
    void UndoDeleteMul(int initline, int endline, std::vector<std::string> originalLines)
    {
        lines = originalLines;
        this->lineInit = initline;
        this->lineEnd = endline;
    }
};

void performUndo(UndoNode *operation, LineNode *&head, LineNode *&current);
void undoInsert(UndoNode *node, LineNode *&linesHead, LineNode *&current);
void undoEdit(UndoNode *node, LineNode *&linesHead, LineNode *&current);
void undoWrite(UndoNode *node, LineNode *&linesHead, LineNode *&current);
void undoDelete(UndoNode *node, LineNode *&linesHead, LineNode *&current);
void undoDeleteMul(UndoNode *node, LineNode *&linesHead, LineNode *&current);
#endif