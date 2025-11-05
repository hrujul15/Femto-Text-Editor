

#include "similarity.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <cstdio> // for remove()

// Split text into lowercase words
static std::unordered_set<std::string> extractWords(const std::string &text)
{
    std::unordered_set<std::string> words;
    std::istringstream ss(text);
    std::string word;

    while (ss >> word)
    {
        // Optional: remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c)
                                  { return std::ispunct(c); }),
                   word.end());

        // Convert to lowercase for case-insensitive hashing
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (!word.empty())
            words.insert(word);
    }
    return words;
}

double compareFiles(const std::string &file1, const std::string &file2)
{
    std::ifstream f1(file1), f2(file2);
    if (!f1.is_open() || !f2.is_open())
    {
        std::cerr << RED << "Error: Could not open one or both files!" << RESET << std::endl;
        return -1.0;
    }

    std::string text1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
    std::string text2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());

    f1.close();
    f2.close();

    auto words1 = extractWords(text1);
    auto words2 = extractWords(text2);

    if (words1.empty() && words2.empty())
        return 100.0;

    // Count intersection
    int common = 0;
    for (const auto &w : words1)
    {
        if (words2.find(w) != words2.end())
            ++common;
    }

    int totalUnique = words1.size() + words2.size() - common;
    double similarity = (static_cast<double>(common) / totalUnique) * 100.0;

    return similarity;
}


void checkFileSimilarity()
{
    std::string file1, file2;
    std::cout << CYAN << "Enter the first file name: " << RESET;
    std::cin >> file1;
    std::cout << CYAN << "Enter the second file name: " << RESET;
    std::cin >> file2;

    double similarity = compareFiles(file1, file2);
    if (similarity < 0)
        return;

    // Print with two decimal places
    std::cout.setf(std::ios::fixed);
    std::cout.precision(2);
    std::cout << YELLOW << "Similarity between files: " << RESET << similarity << "%" << std::endl;
    std::cout.unsetf(std::ios::fixed);

    if (similarity >= 100.0 - 1e-9) 
    {
        char choice;
        std::cout << RED << "Files are identical! Do you want to delete one? (y/n): " << RESET;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            std::string toDelete;
            std::cout << CYAN << "Enter which file to delete (" << file1 << " / " << file2 << "): " << RESET;
            std::cin >> toDelete;
            if (std::remove(toDelete.c_str()) == 0)
                std::cout << GREEN << "File '" << toDelete << "' deleted successfully." << RESET << std::endl;
            else
                std::cout << RED << "Error deleting file!" << RESET << std::endl;
        }
    }
}
