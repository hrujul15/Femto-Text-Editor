#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include <string>

void createAutoSave(const std::string &fileName, const std::string &content);
bool checkAutoSave(const std::string &fileName);
std::string readAutoSave(const std::string &fileName);
void deleteAutoSave(const std::string &fileName);

#endif
