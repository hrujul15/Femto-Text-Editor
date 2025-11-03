#ifndef FIND_AND_REPLACE_H
#define FIND_AND_REPLACE_H
#include <string>
#include <cstddef>
// convert string to lowercase for case-insensitive search
std::string toLower(const std::string &s);
// Find all occurrences of a given word in a file (case-insensitive)
void findWordInFile(const std::string &filePath, const std::string &word);

// Find and display the most frequent word in the file (case-insensitive)
void findMostFrequentWord(const std::string &filePath);

// Find and replace feature(case-sensitive)
void findAndReplaceInFile(const std::string &filePath, const std::string &target, const std::string &replacement);

#endif