#include "mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <vector>

#include "common.hpp"
#include "framework/context.hpp"
#include "objparser.hpp"

using namespace glm;


////////////////////////// Manual mesh loading //////////////////////////

void Mesh::load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    GLsizei stride = 3 * sizeof(float);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(vao.handle, 0);
    glVertexArrayAttribBinding(vao.handle, 0, 0);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);
#endif
}

void Mesh::load(const std::vector<VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    GLsizei stride = sizeof(VertexPCN);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPCN, position));
    glVertexArrayAttribFormat(vao.handle, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPCN, texCoord));
    glVertexArrayAttribFormat(vao.handle, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPCN, normal));
    glEnableVertexArrayAttrib(vao.handle, 0);
    glEnableVertexArrayAttrib(vao.handle, 1);
    glEnableVertexArrayAttrib(vao.handle, 2);
    glVertexArrayAttribBinding(vao.handle, 0, 0);
    glVertexArrayAttribBinding(vao.handle, 1, 0);
    glVertexArrayAttribBinding(vao.handle, 2, 0);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    // Vertex attributes
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCN, position)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCN, texCoord)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCN, normal)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
#endif
}

void Mesh::load(const std::vector<VertexPCNT>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    GLsizei stride = sizeof(VertexPCNT);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPCNT, position));
    glVertexArrayAttribFormat(vao.handle, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPCNT, texCoord));
    glVertexArrayAttribFormat(vao.handle, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPCNT, normal));
    glVertexArrayAttribFormat(vao.handle, 3, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPCNT, tangent));
    glEnableVertexArrayAttrib(vao.handle, 0);
    glEnableVertexArrayAttrib(vao.handle, 1);
    glEnableVertexArrayAttrib(vao.handle, 2);
    glEnableVertexArrayAttrib(vao.handle, 3);
    glVertexArrayAttribBinding(vao.handle, 0, 0);
    glVertexArrayAttribBinding(vao.handle, 1, 0);
    glVertexArrayAttribBinding(vao.handle, 2, 0);
    glVertexArrayAttribBinding(vao.handle, 3, 0);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    // Vertex attributes
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCNT, position)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCNT, texCoord)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCNT, normal)));
    // (location = 3) tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPCNT, tangent)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
#endif
}

void Mesh::load(const std::filesystem::path& filepath) {
    std::vector<VertexPCN> vertices;
    std::vector<unsigned int> indices;
    ObjParser::parse(filepath, vertices, indices);
    load(vertices, indices);
}

void Mesh::loadWithTangents(const std::filesystem::path& filepath) {
    std::vector<VertexPCNT> vertices;
    std::vector<unsigned int> indices;
    ObjParser::parse(filepath, vertices, indices);
    load(vertices, indices);
}

///////////////////////////// Mesh drawing /////////////////////////////

void Mesh::draw() {
    vao.bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::draw(GLsizei instances) {
    vao.bind();
    glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr, instances);
}