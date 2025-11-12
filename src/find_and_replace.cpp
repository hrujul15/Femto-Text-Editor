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

    const int base = 256;   // base for rolling hash
    const int prime = 101;  // a prime number for modulo
    int M = word.length();

 
    if (M == 0)
    {
       std::cout << YELLOW << "Empty search word. Nothing to search." << RESET << "\n";
        return;
    }

    std::string pattern = toLower(word);

    // Compute hash of the pattern
    int patternHash = 0;
    int h = 1; // base^(M-1) % prime

    for (int i = 0; i < M - 1; i++)
    {
        h = (h * base) % prime;
    }

    for (int i = 0; i < M; i++)
    {
        patternHash = (base * patternHash + pattern[i]) % prime;
    }

    std::string line;
    int lineNumber = 0;
    bool found = false;


    while (getline(fin, line))
    {
        lineNumber++;
        std::string lowerLine = toLower(line);
        int N = lowerLine.length();

        if (N < M)
            continue;

        int textHash = 0;

        // Compute initial hash for first window
        for (int i = 0; i < M; i++)
        {
            textHash = (base * textHash + lowerLine[i]) % prime;
        }

        // Slide over the line
        for (int i = 0; i <= N - M; i++)
        {
            if (textHash == patternHash && lowerLine.substr(i, M) == pattern)
            {
                std::cout << GREEN << "Line " << lineNumber << ": " << RESET << line << "\n";
                found = true;                
            }

            // Compute rolling hash for next window
            if (i < N - M)
            {
                textHash = (base * (textHash - lowerLine[i] * h) + lowerLine[i + M]) % prime;
                if (textHash < 0)
                    textHash += prime;
            }
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



// Rolling Hash based Find & Replace
void findAndReplaceInFile(const std::string &filePath, const std::string &findWord, const std::string &replaceWord)
{
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << RED << "Error: Could not open file " << filePath << RESET << "\n";
        return;
    }

    const int base = 256;
    const int prime = 101;
    const int M = findWord.length();

    if (M == 0)
    {
        std::cout << YELLOW << "Empty search word. Nothing to replace." << RESET << "\n";
        return;
    }

    // Precompute hash for pattern
    int patternHash = 0;
    int h = 1; // base^(M-1) % prime
    for (int i = 0; i < M - 1; ++i)
        h = (h * base) % prime;

    for (int i = 0; i < M; ++i)
        patternHash = (base * patternHash + findWord[i]) % prime;

    std::string line;
    std::vector<std::string> updatedLines;
    int replaceCount = 0;
    bool replaced = false;

    while (std::getline(fin, line))
    {
        std::stringstream ss(line);
        std::string word;
        std::string newLine;
        bool firstWord = true;

        // Process line word by word
        while (ss >> word)
        {
            // Rolling hash for this word
            int textHash = 0;
            for (char c : word)
                textHash = (base * textHash + c) % prime;

            // Only replace exact whole word match
            if (textHash == patternHash and word == findWord)
            {
                word = replaceWord;
                replaced = true;
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
              << "' with '" << replaceWord
              << "' successfully." << RESET << "\n";
}