#pragma once

#include <string>
#include <vector>

namespace Common {

std::string readFile(const std::string& filename);
void writeToFile(const std::string& content, const std::string& path);
void filesInDirectory(const std::string& directoryPath, const std::string& extension, std::vector<std::string>& filenames);

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