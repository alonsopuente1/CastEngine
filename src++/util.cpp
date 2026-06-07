#include "util.hpp"

#include <filesystem>
#include <cstring>

void GetAllFilesInDir(const std::string& pPath, std::vector<std::string>& oFiles)
{
    for (const auto& entry : std::filesystem::directory_iterator(pPath))
        if (entry.is_regular_file())
            oFiles.push_back(entry.path().string());
}

void fileNameFromPath(const char* path, char* outName, int maxLen)
{
    char* lastSlash = (char*)path;
    char* lastDot = (char*)path;
    for(char* p = (char*)path; *p != '\0'; p++)
    {
        if(*p == '/' || *p == '\\')
            lastSlash = p + 1;
        if(*p == '.')
            lastDot = p;
    }

    int charsToCopy = lastDot - lastSlash;

    if(charsToCopy >= maxLen)
        charsToCopy = maxLen - 1;
    if(charsToCopy <= 0)
        return;

    memcpy(outName, lastSlash, charsToCopy);
    outName[charsToCopy] = '\0';
}