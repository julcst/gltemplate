#pragma once

#include <string>
#include <vector>

namespace Common {

std::string readFile(const std::string& filename);
void writeToFile(const std::string& content, const std::string& path);
void filesInDirectory(const std::string& directoryPath, const std::string& extension, std::vector<std::string>& filenames);

}