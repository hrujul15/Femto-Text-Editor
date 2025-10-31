// Function declarations of file_ops.h
#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <string>
#include <fstream>
#include <iostream>
#include "util.h"

int readFile(std::string fileName);
int writeFile(std::string fileName);

#endif