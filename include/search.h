#ifndef SEARCH_H
#define SEARCH_H

#include <string>

// Find all occurrences of a given word in a file (case-insensitive)
void findWordInFile(const std::string &filePath, const std::string &word);

// Find and display the most frequent word in the file (case-insensitive)
void findMostFrequentWord(const std::string &filePath);

#endif 