#include "clipboard.h"
#include <fstream>
#include <iostream>

std::vector<std::string> clipboardBuffer;

// Copies a range of lines (startLine → endLine) from the given file into the global clipboard buffer
void copyFromFile(const std::string& filePath, int startLine, int endLine)
{
    clipboardBuffer.clear();
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        std::cout << "Error: Could not open file " << filePath << "\n";
        return;
    }

    std::string line;
    int lineCount = 1;
    while (std::getline(fin, line)) {
        if (lineCount >= startLine && lineCount <= endLine) {
            clipboardBuffer.push_back(line);
        }
        if (lineCount > endLine) break;
        lineCount++;
    }
    fin.close();
    std::cout << "Copied " << clipboardBuffer.size() << " lines to clipboard.\n";
}

// Pastes the contents of the global clipboard buffer into the specified file at the given line number
void pasteToFile(const std::string& filePath, int lineNumber)
{
    if (clipboardBuffer.empty()) {
        std::cout << "Clipboard is empty!\n";
        return;
    }

    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        std::cout << "Error: Could not open file " << filePath << "\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    fin.close();

    if (lineNumber < 1) lineNumber = 1;
    if (lineNumber > (int)lines.size() + 1) lineNumber = lines.size() + 1;

    lines.insert(lines.begin() + (lineNumber - 1), clipboardBuffer.begin(), clipboardBuffer.end());

    std::ofstream fout(filePath);
    for (auto& l : lines) fout << l << "\n";
    fout.close();

    std::cout << "Pasted clipboard contents into " << filePath << " at line " << lineNumber << ".\n";
}
