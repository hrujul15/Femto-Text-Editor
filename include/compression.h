#ifndef COMPRESSION_H
#define COMPRESSION_H
#include "util.h"
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <iostream>
struct Node
{
    // Build a node to store the huffmann tree
    char key;
    int freq;
    Node *left;
    Node *right;
    std::string code;
    Node(char key, int freq)
    {
        this->freq = freq;
        this->key = key;
        left = nullptr;
        right = nullptr;
    }
    Node(char key, int freq, Node *left, Node *right)
    {
        this->freq = freq;
        this->key = key;
        this->left = left;
        this->right = right;
    }
};
struct Compare
{
    bool operator()(Node *a, Node *b)
    {
        return a->freq > b->freq;
    }
};

int readFromBinary();
void assignCode(Node *node, std::ofstream &binFile, std::unordered_map<char, std::string> &characterCode, std::string code = "");
int readFileforCompression();
#endif