// Function declarations of file_ops.h
#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"
#include <limits>

int readFile(std::string fileName);
int writeFile(std::string fileName);
int editFile(std::string fileName);
// Linked List Node
struct LineNode
{
    std::string line;
    LineNode *nextLine = nullptr;
    LineNode(std::string line)
    {
        this->line = line;
        nextLine = nullptr;
    }
};
#endif