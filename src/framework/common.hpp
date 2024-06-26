#pragma once

#include <string>
#include <vector>
#include <filesystem>

/**
 * @file common.hpp
 * @brief Defines common functions for file I/O and hashing.
 */
namespace Common {

    /**
     * @brief Reads the content of a file.
     * @param filepath The path to the file.
     * @return The content of the file.
     */
    std::string readFile(const std::filesystem::path& filepath);

    /**
     * @brief Writes content to a file.
     * @param content The content to write.
     * @param filepath The path to the file.
     */
    void writeToFile(const std::string& content, const std::filesystem::path& filepath);
    void filesInDirectory(const std::filesystem::path& directoryPath, const std::string& extension, std::vector<std::filesystem::path>& filenames);

    template <class T, typename... Rest>
    void hash_combine(std::size_t& seed, const T& v, const Rest&... rest);

}

/**
 * Hash combine function from boost
 * Source: https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
 */
template <class T, typename... Rest>
inline void Common::hash_combine(std::size_t& seed, const T& v, const Rest&... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    (hash_combine(seed, rest), ...);
}