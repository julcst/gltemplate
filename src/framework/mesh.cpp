#include "mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "common.hpp"
#include "config.hpp"
#include "objparser.hpp"

using namespace glm;


////////////////////////// Manual mesh loading //////////////////////////

void Mesh::load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    size_t stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    vao.unbind();
}

void Mesh::load(const std::vector<VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    // Vertex attributes
    size_t stride = sizeof(VertexPCN);
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    vao.unbind();
}

void Mesh::load(const std::vector<VertexPCNT>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    // Vertex attributes
    size_t stride = sizeof(VertexPCNT);
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(float)));
    // (location = 3) tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*) (8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    vao.unbind();
}

void Mesh::load(const std::string& filepath) {
    std::vector<VertexPCN> vertices;
    std::vector<unsigned int> indices;
    ObjParser::parse(filepath, vertices, indices);
    load(vertices, indices);
}

void Mesh::loadWithTangents(const std::string& filepath) {
    std::vector<VertexPCNT> vertices;
    std::vector<unsigned int> indices;
    ObjParser::parse(filepath, vertices, indices);
    load(vertices, indices);
}

///////////////////////////// Mesh drawing /////////////////////////////

void Mesh::draw() {
    vao.bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    vao.unbind();
}