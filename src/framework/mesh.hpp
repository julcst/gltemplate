#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "gl/buffer.hpp"
#include "gl/vertexarray.hpp"

const std::vector<float> FULLSCREEN_VERTICES = {
    -1.0f, -1.0f, 0.0f,
     3.0f, -1.0f, 0.0f,
    -1.0f,  3.0f, 0.0f,
};

const std::vector<unsigned int> FULLSCREEN_INDICES = {
    0, 1, 2,
};

class Mesh {
   public:
    /**
     * Vertex with 3 position components, 2 texture coordinate components, 3 normal vector components
     */
    struct VertexPCN {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };
    /**
     * Vertex with 3 position components, 2 texture coordinate components, 3 normal vector components, 3 tangent vector components
     */
    struct VertexPCNT {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    void load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPCN>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPCNT>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::string& filepath);
    void loadWithTangents(const std::string& filepath);
    void draw();
    
    unsigned int numIndices = 0;
    VertexArray vao;
    Buffer vbo;
    Buffer ebo;
};
