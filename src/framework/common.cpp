#include "common.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "framework/context.hpp"

std::string Common::readFile(const std::filesystem::path& filepath) {
    std::ifstream stream{filepath};
    std::cout << "Loading " << std::filesystem::absolute(filepath) << std::endl;
    if (!stream.is_open()) throw std::runtime_error("Could not open file: " + std::filesystem::absolute(filepath).string() + "\n" + Context::getCWDWarning());
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::vector<char> Common::readBinaryFile(const std::filesystem::path& filepath) {
    std::ifstream stream{filepath, std::ios::binary};
    std::cout << "Loading " << std::filesystem::absolute(filepath) << std::endl;
    if (stream.fail()) throw std::runtime_error("Could not open file: " + std::filesystem::absolute(filepath).string() + "\n" + Context::getCWDWarning());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

void Common::writeToFile(std::string_view content, const std::filesystem::path& filepath) {
    std::filesystem::create_directories(filepath.parent_path());
    std::ofstream out{filepath};
    std::cout << "Writing " << std::filesystem::absolute(filepath) << std::endl;
    if (!out.is_open()) throw std::runtime_error("Could not open file: " + std::filesystem::absolute(filepath).string());
    out << content;
    out.close();
}

void Common::filesInDirectory(const std::filesystem::path& directoryPath, std::string_view extension, std::vector<std::filesystem::path>& filenames) {
    if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry.status()) && entry.path().extension() == extension) {
                filenames.push_back(entry.path().relative_path());
            } 
        }
    }
}