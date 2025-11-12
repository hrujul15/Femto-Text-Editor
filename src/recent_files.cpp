#include "recent_files.h"
#include "util.h"
#include <iostream>
#include <algorithm>
#include <fstream>
// Global deque to store recent files (in memory)
static std::deque<std::string> recentFiles;

// Function: addRecentFile
// Adds a filename to the list, avoids duplicates, and keeps only 5 recent entries
void addRecentFile(const std::string &fileName)
{
    //  Check if the file actually exists before adding
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cout << RED << "Warning: '" << fileName 
                  << "' does not exist.Skipping from recent list." << RESET << std::endl;
        return;
    }
    file.close();

    // Remove if already exists (avoid duplicates)
    auto it = std::find(recentFiles.begin(), recentFiles.end(), fileName);
    if (it != recentFiles.end())
        recentFiles.erase(it);

    // Add new file to front
    recentFiles.push_front(fileName);

    // Keep only 5 entries
    if (recentFiles.size() > 5)
        recentFiles.pop_back();
}


// Function: showRecentFiles
// Displays up to 5 most recently accessed files
void showRecentFiles()
{
    if (recentFiles.empty())
    {
        std::cout << YELLOW << "No recent files found!" << RESET << std::endl;
        return;
    }

    std::cout << CYAN << "Recent Files for this Session(oldest first, most recent last):" << RESET << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    int index = 1;
   
    for (auto it = recentFiles.rbegin(); it != recentFiles.rend(); ++it)
    {
        std::cout << GREEN << index++ << ". " << RESET << *it << std::endl;
    }

    std::cout << "-----------------------------------" << std::endl;
}
