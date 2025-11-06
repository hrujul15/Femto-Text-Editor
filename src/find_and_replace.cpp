#include "find_and_replace.h"
#include "util.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cstddef>


//convert string to lowercase for case-insensitive search
std::string toLower(const std::string &s)
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
    return;
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
    return;
}

// Delete all occurrences of a specific word from the file (case-sensitive)
void deleteWordFromFile(const std::string &filePath, const std::string &targetWord)
{
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << RED << "Error: Could not open file " << filePath << RESET << "\n";
        return;
    }

    std::string line;
    std::vector<std::string> updatedLines;
    bool found = false;
    int deletedCount = 0;

    // Go through every line in the file
    while (std::getline(fin, line))
    {
        std::stringstream ss(line);
        std::string word;
        std::string newLine;
        bool firstWord = true;

        
        while (ss >> word)
        {
            if (word == targetWord)
            {
                found = true;
                deletedCount++;
                continue;
            }

            if (!firstWord)
                newLine += " ";
            newLine += word;
            firstWord = false;
        }

        updatedLines.push_back(newLine);
    }

    fin.close();

    if (!found)
    {
        std::cout << YELLOW << "Word '" << targetWord << "' not found in file." << RESET << "\n";
        return;
    }

    // Rewrite file with updated content
    std::ofstream fout(filePath, std::ios::trunc);
    for (const auto &ln : updatedLines)
    {
    
    if (!ln.empty())
        fout << ln << "\n";
    }
    fout.close();

    std::cout << GREEN << "Deleted all " << deletedCount 
          << " occurrences of '" << targetWord 
          << "' successfully!" << RESET << "\n";

}

//Find and Replace word in the file
void findAndReplaceInFile(const std::string &filePath, const std::string &findWord, const std::string &replaceWord)
{
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << RED << "Error: Could not open file " << filePath << RESET << "\n";
        return;
    }

    std::string line;
    std::vector<std::string> updatedLines;
    bool replaced = false;
    int replaceCount = 0;

    while (std::getline(fin, line))
    {
        std::stringstream ss(line);
        std::string word;
        std::string newLine;
        bool firstWord = true;

        // Go word by word to ensure whole-word match
        while (ss >> word)
        {
            if (word == findWord)
            {
                replaced = true;
                word = replaceWord;
                replaceCount++;
            }

            if (!firstWord)
                newLine += " ";
            newLine += word;
            firstWord = false;
        }

        updatedLines.push_back(newLine);
    }

    fin.close();

    if (!replaced)
    {
        std::cout << YELLOW << "Word '" << findWord << "' not found in file." << RESET << "\n";
        return;
    }

    // Rewrite file
    std::ofstream fout(filePath, std::ios::trunc);
    for (const auto &ln : updatedLines)
        fout << ln << "\n";
    fout.close();

    std::cout << GREEN << "Replaced " << replaceCount 
              << " occurrence(s) of '" << findWord 
              << "' with '" << replaceWord << "' successfully!" << RESET << "\n";
}
