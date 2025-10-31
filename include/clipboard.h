#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>
#include <vector>

// The clipboard stores copied lines temporarily
extern std::vector<std::string> clipboardBuffer;

// Copy lines from a file
void copyFromFile(const std::string& filePath, int startLine, int endLine);

// Paste clipboard contents into a file at a given line
void pasteToFile(const std::string& filePath, int lineNumber);

#endif
