#include <filesystem>
#include <vector>

#include "mesh.hpp"

namespace ObjParser {
    /**
     * @brief Parses a Wavefront OBJ file with position, color, and normal vectors.
     * @note `vertices` and `indices` are output parameters and not cleared before adding new data.
     */
    void parse(const std::filesystem::path& filepath, std::vector<Mesh::VertexPTN>& vertices, std::vector<unsigned int>& indices);

    /**
     * @brief Parses a Wavefront OBJ file with position, color, and normal vectors and generates tangent vectors.
     * @note `vertices` and `indices` are output parameters and not cleared before adding new data.
     */
    void parse(const std::filesystem::path& filepath, std::vector<Mesh::VertexPTNT>& vertices, std::vector<unsigned int>& indices);
}