#include <filesystem>
#include <vector>

#include "mesh.hpp"

namespace ObjParser {
    void parse(const std::filesystem::path& filepath, std::vector<Mesh::VertexPCN>& vertices, std::vector<unsigned int>& indices);
    void parse(const std::filesystem::path& filepath, std::vector<Mesh::VertexPCNT>& vertices, std::vector<unsigned int>& indices);
}