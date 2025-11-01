#include "search.h"
#include "util.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <iostream>

//convert string to lowercase for case-insensitive search
static std::string toLower(const std::string &s)
{
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

//Find all occurrences of a word in the file
void findWordInFile(const std::string &filePath, const std::string &word)
{
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << RED << "Error: Could not open file " << filePath << RESET << "\n";
        return;
    }

    std::string line;
    int lineNumber = 0;
    bool found = false;
    std::string target = toLower(word);

    std::cout << CYAN << "\nSearching for '" << word << "' in " << filePath << "...\n"
              << RESET;

    while (std::getline(fin, line))
    {
        lineNumber++;
        std::string lowerLine = toLower(line);
        if (lowerLine.find(target) != std::string::npos)
        {
            std::cout << GREEN << "Line " << lineNumber << ": " << RESET << line << "\n";
            found = true;
        }
    }

    if (!found)
        std::cout << YELLOW << "No matches found for '" << word << "'." << RESET << "\n";

    fin.close();
}

// Find the most frequent word in the file
void findMostFrequentWord(const std::string &filePath)
{
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << RED << "Error: Could not open file " << filePath << RESET << "\n";
        return;
    }

    std::map<std::string, int> wordCount;
    std::string word;

    while (fin >> word)
    {
        std::string cleanWord;
        for (char c : word)
        {
            if (std::isalpha(static_cast<unsigned char>(c)))
                cleanWord += std::tolower(c);
        }
        if (!cleanWord.empty())
            wordCount[cleanWord]++;
    }

    fin.close();

    if (wordCount.empty())
    {
        std::cout << YELLOW << "File is empty or contains no valid words." << RESET << "\n";
        return;
    }

    std::string mostFrequent;
    int maxCount = 0;

    for (const auto &entry : wordCount)
    {
        if (entry.second > maxCount)
        {
            mostFrequent = entry.first;
            maxCount = entry.second;
        }
    }

    std::cout << CYAN << "Most frequent word: " << WHITE << mostFrequent
              << CYAN << " (" << maxCount << " times)" << RESET << "\n";
}
