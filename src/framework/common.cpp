#include "common.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::string Common::readFile(const std::string& filepath) {
    std::filesystem::path path(filepath);
    std::ifstream stream(path);
    if (!stream.is_open()) throw std::runtime_error("Could not open file: " + path.string() + " relative to current working directory: " + std::filesystem::current_path().string());
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

void Common::writeToFile(const std::string& content, const std::string& filepath) {
    std::filesystem::path path(filepath);
    std::filesystem::create_directories(path.parent_path());
    std::ofstream out(path);
    if (!out.is_open()) throw std::runtime_error("Could not open file: " + path.string() + " relative to current working directory: " + std::filesystem::current_path().string());
    out << content;
    out.close();
}

void Common::filesInDirectory(const std::string& directoryPath, const std::string& extension, std::vector<std::string>& filenames) {
    if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry.status()) && entry.path().extension() == extension) {
                filenames.push_back(entry.path().relative_path().string());
            } 
        }
    }
}