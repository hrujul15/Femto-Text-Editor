#include "stats.h"
#include "util.h"

//Count total words and characters
int countStats(const std::string &fileName)
{
    std::ifstream file(fileName);

    if(!file.is_open())
    {
        std::cerr << RED << "Error: Could not open file " << RESET << std::endl;
        return 1;
    }

    int wordCount=0;
    int charCount=0;
    char c;
    bool inWord=false;

    //Read file character by character
    while(file.get(c))
    {
        if (!std::isspace(static_cast<unsigned char>(c)))  // Ignores space,tab,newline
           ++charCount;

        
        if(std::isspace(static_cast<unsigned char>(c)))  // A word starts when we encounter a non-space after a space or newline
        {
            if(inWord)
            {
                ++wordCount;
                inWord = false;
            }
        }
        else
        {
         inWord=true;
        }    
            
    }


if(inWord)
   ++wordCount;

std::cout << YELLOW << "Total Characters:" << RESET << charCount << std::endl;
std::cout << YELLOW << "Total Words:" << RESET << wordCount << std::endl;  

file.close();
return 0;
}