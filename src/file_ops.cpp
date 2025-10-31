// This will handle file read & write operations
#include "file_ops.h"

int readFile(std::string fileName)
{
    //  Open the text file
    std::ifstream file(fileName);

    // Error opening the file
    if (!file.is_open())
    {
        std::cerr << "Error Opening the File!!!" << std::endl;
        return 1;
    }

    // Storing contents of a line in a string then printing it
    std::string line;
    while (std::getline(file, line))
        std::cout << line << std::endl;

    file.close();
    return 0;
}

int writeFile(std::string fileName)
{

    // Create the file
    std::ofstream file(fileName);

    // Error opening the file
    if (!file.is_open())
    {
        std::cerr << "Error Opening the File!!!" << std::endl;
        return 1;
    }
    

    // Write to file
    std::string line;
    do
    {
        std::getline(std::cin, line);
        if (line != "/cmd")
            file << line << std::endl;
    } while (line != "/cmd");
    return 0;
}
