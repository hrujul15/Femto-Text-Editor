// Function declarations of file_ops.h
#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"
#include <limits>
#include <stack>
int readFile(std::string fileName);
int writeFile(std::string fileName, bool editing = false);
int editFile(std::string fileName);

// Linked List Node


int traverseAndPrint(LineNode *trav);

struct Action
{
    enum Ops
    {
        ADD,
        EDIT,
        INSERT,
        DELETE
    } type;
};
#endif