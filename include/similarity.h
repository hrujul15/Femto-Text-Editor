#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <cstdio>

double compareFiles(const std::string &file1, const std::string &file2);
static std::unordered_set<std::string> extractWords(const std::string &text);
void checkFileSimilarity();

#endif
