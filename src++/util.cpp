#include "util.hpp"

#include <filesystem>

void GetAllFilesInDir(const std::string& pPath, std::vector<std::string>& oFiles)
{
    for (const auto& entry : std::filesystem::directory_iterator(pPath))
        if (entry.is_regular_file())
            oFiles.push_back(entry.path().string());
}