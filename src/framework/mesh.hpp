#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <vector>

#include "gl/buffer.hpp"
#include "gl/vertexarray.hpp"

class Mesh {
   public:

    /**
     * Vertices of a fullscreen triangle
     */
    inline static const std::vector<float> FULLSCREEN_VERTICES {
        -1.0f, -1.0f, 0.0f,
         3.0f, -1.0f, 0.0f,
        -1.0f,  3.0f, 0.0f,
    };

    /**
     * Indices of a fullscreen triangle
     */
    inline static const std::vector<unsigned int> FULLSCREEN_INDICES {
        0, 1, 2,
    };

    /**
     * Vertex with 3 position components, 3 color components
     */
    struct VertexPC {
        glm::vec3 position;
        glm::vec3 color;
    };

    /**
     * Vertex with 3 position components, 2 texture coordinate components, 3 normal vector components
     */
    struct VertexPTN {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };
    
    /**
     * Vertex with 3 position components, 2 texture coordinate components, 3 normal vector components, 3 tangent vector components
     */
    struct VertexPTNT {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    void load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<float>& vertices, const std::vector<unsigned int>& attributeSizes, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPC>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPTN>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::vector<VertexPTNT>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::filesystem::path& filepath);
    void loadWithTangents(const std::filesystem::path& filepath);
    void draw();
    void draw(GLsizei instances);
    
    GLsizei numIndices = 0;
    VertexArray vao;
    Buffer<GL_ARRAY_BUFFER> vbo;
    Buffer<GL_ELEMENT_ARRAY_BUFFER> ebo;
};
