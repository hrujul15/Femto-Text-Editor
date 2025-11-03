#include "autosave.h"
#include <fstream>
#include <iostream>
#include <cstdio>  
#include <string>

void createAutoSave(const std::string &fileName, const std::string &content) {
    std::string backupFile = fileName + ".autosave";
    std::ofstream out(backupFile);
    if (out.is_open()) {
        out << content;
        out.close();
        
    } else {
        std::cerr << "Error: Could not create autosave file: " << backupFile << std::endl;
    }
}

bool checkAutoSave(const std::string &fileName) {
    std::string backupFile = fileName + ".autosave";
    std::ifstream in(backupFile);
    return in.good(); // file exists if stream opens successfully
}

std::string readAutoSave(const std::string &fileName) {
    std::string backupFile = fileName + ".autosave";
    std::ifstream in(backupFile);
    if (!in.is_open()) {
        std::cerr << "Error: Could not open autosave file." << std::endl;
        return "";
    }

    std::string data, line;
    while (std::getline(in, line))
        data += line + "\n";
    in.close();
    return data;
}

void deleteAutoSave(const std::string &fileName) {
    std::string backupFile = fileName + ".autosave";
    if (checkAutoSave(fileName)) {
        if (std::remove(backupFile.c_str()) == 0)
            std::cout << "[AutoSave] Backup deleted: " << backupFile << std::endl;
        else
            std::cerr << "Error: Could not delete autosave file." << std::endl;
    }
}
