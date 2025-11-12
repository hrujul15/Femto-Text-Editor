#include "compression.h"
#include <filesystem>
// #include "util.h"
int readFromBinary()
{
    std::string fileName;
    std::cout << CYAN << "Type the path to the file to De-Compress: " << RESET << std::endl;
    std::cin >> fileName;
    std::cin.ignore(); // To clear input buffer
    fileName = fileName.substr(0, fileName.size() - 4);
    std::ifstream binFile(fileName + ".bin", std::ios::binary);
    std::ofstream newFile(fileName + "_Copy.txt");
    if (!binFile.is_open())
    {
        std::cerr << RED << "Error Opening the File!!!" << RESET << std::endl;
        return 1;
    }

    std::string line;

    // New Map to access the codes for the specfic char
    std::unordered_map<std::string, char> codeChar;
    int padding = 0;

    while (std::getline(binFile, line))
    {

        if (line == "===")
            break;
        int space_pos = line.find(" ");
        if (line.substr(0, space_pos) == "PADDING")
        {
            padding = stoi(line.substr(space_pos + 1, line.size()));
        }
        else
        {
            char character = stoi(line.substr(0, space_pos));
            std::string code = line.substr(space_pos + 1, line.size());
            codeChar[code] = character;
        }
    }

    // Getting the binary text line!
    // storing text in buffer
    std::string buffer;
    // read binary data a byte at a time convert to charecters
    // unsigned cuz we read 1 byte at a time
    unsigned char byte;
    // reinterept cast to change label from unsigned char to char
    while (binFile.read(reinterpret_cast<char *>(&byte), 1))
    {
        // data in the byte was inserted right to left so loop from right to left
        for (int i = 7; i >= 0; i--)
        {
            // temporarily right shift the bit, AND with 1 depening on result
            // push 1 or 0 to buffer
            buffer += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    std::string testCode;
    // resize buffer to remove the padding bits
    buffer = buffer.substr(0, buffer.size() - padding);
    for (int c : buffer)
    {
        // adding each bit to string
        testCode += c;
        // if code matches the one in the map, output the char to file
        if (codeChar.find(testCode) != codeChar.end())
        {
            char ch = codeChar[testCode];
            newFile.put(codeChar[testCode]);
            testCode = "";
        }
    }
    std::cout << CYAN << "New file saved as: " << fileName + "_Copy.txt" << RESET << std::endl;
    binFile.close();
    return 0;
}

void assignCode(Node *node, std::ofstream &binFile, std::unordered_map<char, std::string> &characterCode, std::string code)
{
    if (!node)
        return;
    if (node->key != '$')
    {
        node->code = code;
        // Putting codes to binary file
        binFile << (int)node->key << ' ' << node->code << std::endl;
        characterCode[node->key] = node->code;
    }
    assignCode(node->left, binFile, characterCode, code + "0");
    assignCode(node->right, binFile, characterCode, code + "1");
}

int readFileforCompression()
{
    std::string fileName;
    std::cout << CYAN << "Type the path to the file to compress: " << RESET << std::endl;
    std::cin >> fileName;
    std::cin.ignore(); // To clear input buffer
    std::string outputfileName = fileName.substr(0, fileName.size() - 4);
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

    // New Map to access the codes for the specfic char
    std::unordered_map<char, std::string> characterCode;

    // Reading lines and adding it to unordered map
    std::string line;
    while (std::getline(file, line))
    {
        for (char c : line)
        {
            frequency[c]++;
        }

        // Add frequency of '\n'
        frequency['\n']++;
    }

    // Now add the {char, frequency} to priority queue
    for (auto it : frequency)
    {
        Node *node = new Node(it.first, it.second);
        heap.push(node);
    }
    std::cout << RESET;

    while (heap.size() != 1 and heap.size() != 0)
    {
        Node *left = heap.top();
        heap.pop();
        Node *right = heap.top();
        heap.pop();
        Node *newer = new Node('$', left->freq + right->freq, left, right);
        heap.push(newer);
    }
    // Empty heap that is provided file is empty!
    if (heap.empty())
    {
        std::cout << RED << "This file is either empty or in a format that cannot be read." << RESET << std::endl;
        return 1;
    }
    Node *root = heap.top();

    std::ofstream binFile(outputfileName + ".bin", std::ios::binary);

    if (!binFile.is_open())
    {
        std::cerr << RED << "Error Opening the File!!!" << RESET << std::endl;
        return 1;
    }
    assignCode(root, binFile, characterCode, "");

    // Now codes are put into binary file, put the text to binary file as encoded bits

    file.clear(); // clears EOF and fail bits
    file.seekg(0);
    std::string line2;
    std::string buffer;
    while (std::getline(file, line2))
    {
        for (char c : line2)
        {
            buffer += characterCode[c];
        }
        // getline drops the '\n' char so manually adding it
        buffer += characterCode['\n'];
    }

    // get size of padding required
    size_t lineSize = buffer.size();
    int padding = 8 - (lineSize % 8);

    // pushing the req padding
    binFile << "PADDING" << ' ' << padding << std::endl;
    binFile << "===" << std::endl;

    // Now pushing bytes to binary file
    unsigned char byte = 0;
    int bitCount = 0;
    for (char bit : buffer)
    {
        // byte = CURRENT BYTE (OR) bit == 1, ie if bit = 1, then BYTE OR bit will put 1
        // as the least significant bit so on
        byte = (byte << 1) | (bit == '1');
        bitCount++;
        // if a byte is completed,
        if (bitCount == 8)
        {
            binFile.put(byte);
            byte = 0;
            bitCount = 0;
        }
    }

    // if last few char put but padding incomplete
    if (bitCount > 0)
    {
        byte <<= (8 - bitCount);
        binFile.put(byte);
    }

    double ogSize = std::filesystem::file_size(std::filesystem::path(fileName)) / 1024.00;
    double newSize = std::filesystem::file_size(std::filesystem::path(outputfileName + ".bin")) / 1024.00;
    double compressionRatio = (newSize / ogSize) * 100.0;
    compressionRatio = 100.00 - compressionRatio;
    if (compressionRatio < 5.00)
    {
        std::cout << RED << "File Compression Unsuccessful!" << std::endl;
        std::cout << RED << "Due to file size being very small, pick a large file!" << RESET << std::endl;
    }
    else
    {

        std::cout << CYAN << "File Compression Successful!" << std::endl;
        std::cout << "Original File Size: " << ogSize << " KB" << std::endl;
        std::cout << "New File Size: " << newSize << " KB" << std::endl;
        std::cout << "Compression: " << (compressionRatio) << "% smaller" << std::endl;
        std::cout << "(Note: Reported sizes are logical file sizes\nActual disk usage may be higher due to filesystem block allocation)" << RESET << std::endl;
    }
    file.close();
    binFile.close();
    // readFromBinary(outputfileName);

    return 0;
}

// for debugging purposes before adding to main code
// int main()
// {
//     readFile("Femto-Text-Editor/a.txt");
// }