#pragma once

#include <string>
#include <vector>

bool GetAllFilesInDir(const std::string& pPath, std::vector<std::string>& oFiles);
void fileNameFromPath(const char* path, char* outName, int maxLen);