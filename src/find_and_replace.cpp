#include "find_and_replace.h"
#include "util.h"

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

//Find and Replace word in the file
void findAndReplaceInFile(const std::string &filePath, const std::string &target, const std::string &replacement)
{
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << RED << "Error: Could not open file " << filePath << RESET << "\n";
        return;
    }

    std::string content;
    std::string line;
    int replaceCount = 0;

    // Read the file line by line
    while (std::getline(fin, line))
    {
        size_t pos = 0;
        
        // Replace all occurrences of target in this line
        while ((pos = line.find(target, pos)) != std::string::npos)
        {
            line.replace(pos, target.length(), replacement);
            pos += replacement.length();
            ++replaceCount;
        }
        content += line + "\n";
    }

    fin.close();

    // Write the updated content back into the file
    std::ofstream fout(filePath);
    if (!fout.is_open())
    {
        std::cout << RED << "Error: Could not write to file " << filePath << RESET << "\n";
        return;
    }

    fout << content;
    fout.close();

    if (replaceCount > 0)
        std::cout << GREEN << "Replaced " << replaceCount << " occurrence(s) of '"
                  << target << "' with '" << replacement << "'." << RESET << "\n";
    else
        std::cout << YELLOW << "No occurrences of '" << target << "' found in the file." << RESET << "\n";
}
