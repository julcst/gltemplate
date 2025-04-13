#include "mesh.hpp"

#include <glad/gl.h>
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

void Mesh::load(const std::vector<float>& vertices, const std::vector<unsigned int>& attributeSizes, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    GLsizei stride = 0;
    for (const auto attributeSize : attributeSizes) {
        stride += attributeSize * sizeof(float);
    }

#ifdef MODERN_GL
    GLuint offset = 0;
    for (unsigned int attributeIdx = 0; attributeIdx < attributeSizes.size(); attributeIdx++) {
        const auto attributeSize = attributeSizes[attributeIdx];
        glEnableVertexArrayAttrib(vao.handle, attributeIdx);
        glVertexArrayAttribBinding(vao.handle, attributeIdx, 0);
        glVertexArrayAttribFormat(vao.handle, attributeIdx, attributeSize, GL_FLOAT, GL_FALSE, offset);
        offset += attributeSize * sizeof(float);
    }
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    GLuint offset = 0;
    for (unsigned int attributeIdx = 0; attributeIdx < attributeSizes.size(); attributeIdx++) {
        const auto attributeSize = attributeSizes[attributeIdx];
        glEnableVertexAttribArray(attributeIdx);
        glVertexAttribPointer(attributeIdx, attributeSize, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
        offset += attributeSize * sizeof(float);
    }
#endif
}

void Mesh::load(const std::vector<VertexPC>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    GLsizei stride = sizeof(VertexPC);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPC, position));
    glVertexArrayAttribFormat(vao.handle, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPC, color));
    glEnableVertexArrayAttrib(vao.handle, 0);
    glEnableVertexArrayAttrib(vao.handle, 1);
    glVertexArrayAttribBinding(vao.handle, 0, 0);
    glVertexArrayAttribBinding(vao.handle, 1, 0);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    // Vertex attributes
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPC, position)));
    // (location = 1) color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPC, color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
#endif
}

void Mesh::load(const std::vector<VertexPTN>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    GLsizei stride = sizeof(VertexPTN);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPTN, position));
    glVertexArrayAttribFormat(vao.handle, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPTN, texCoord));
    glVertexArrayAttribFormat(vao.handle, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPTN, normal));
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTN, position)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTN, texCoord)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTN, normal)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
#endif
}

void Mesh::load(const std::vector<VertexPTNT>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices, GL_STATIC_DRAW);
    ebo.load(indices, GL_STATIC_DRAW);

    // Bind buffers to VAO
    GLsizei stride = sizeof(VertexPTNT);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPTNT, position));
    glVertexArrayAttribFormat(vao.handle, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexPTNT, texCoord));
    glVertexArrayAttribFormat(vao.handle, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPTNT, normal));
    glVertexArrayAttribFormat(vao.handle, 3, 3, GL_FLOAT, GL_FALSE, offsetof(VertexPTNT, tangent));
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTNT, position)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTNT, texCoord)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTNT, normal)));
    // (location = 3) tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(VertexPTNT, tangent)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
#endif
}

void Mesh::load(const std::filesystem::path& filepath) {
    std::vector<VertexPTN> vertices;
    std::vector<unsigned int> indices;
    ObjParser::parse(filepath, vertices, indices);
    load(vertices, indices);
}

void Mesh::loadWithTangents(const std::filesystem::path& filepath) {
    std::vector<VertexPTNT> vertices;
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