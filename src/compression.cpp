#include "compression.h"
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "util.h"
#define print(s) cout << (s) << endl
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
void assignCode(Node *node, std::string code = "")
{
    if (!node)
        return;
    if (node->key != '$')
    {
        node->code = code;
    }
    assignCode(node->left, code + "0");
    assignCode(node->right, code + "1");
}
struct Compare
{
    bool operator()(Node *a, Node *b)
    {
        return a->freq > b->freq;
    }
};
int readFile(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << RED << "Error Opening the File!!!" << RESET << std::endl;
        return 1;
    }
    // Make a priority queue to store elements with lowest frequency at top
    std::priority_queue<Node *, std::vector<Node *>, Compare> heap;
    // Unordered map to store character frequency
    std::unordered_map<char, int> frequency;

    // Reading lines and adding it to unordered map
    std::string line;
    while (std::getline(file, line))
    {
        for (char c : line)
        {
            frequency[c]++;
        }
    }

    // Now add the {char, frequency} to priority queue
    for (auto it : frequency)
    {
        Node *node = new Node(it.first, it.second);
        heap.push(node);
    }
    file.close();
    std::cout << RESET;

    while (heap.size() != 1)
    {
        Node *left = heap.top();
        heap.pop();
        Node *right = heap.top();
        heap.pop();
        Node *newer = new Node('$', left->freq + right->freq, left, right);
        heap.push(newer);
    }
    Node *root = heap.top();

    assignCode(root, "");
    return 0;
}