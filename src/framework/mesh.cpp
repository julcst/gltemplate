#include "mesh.hpp"

#include <glbinding/gl/gl.h>
using namespace gl;
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
    vbo.load(vertices);
    ebo.load(indices);

    // Bind buffers to VAO
    size_t stride = 3 * sizeof(float);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, (void*) (0 * sizeof(float)));
    glEnableVertexArrayAttrib(vao.handle, 0);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);
#endif
}

void Mesh::load(const std::vector<VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices);
    ebo.load(indices);

    // Bind buffers to VAO
    size_t stride = sizeof(VertexPCN);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, (void*) (0 * sizeof(float)));
    glVertexArrayAttribFormat(vao.handle, 1, 2, GL_FLOAT, GL_FALSE, (void*) (3 * sizeof(float)));
    glVertexArrayAttribFormat(vao.handle, 2, 3, GL_FLOAT, GL_FALSE, (void*) (5 * sizeof(float)));
    glEnableVertexArrayAttrib(vao.handle, 0);
    glEnableVertexArrayAttrib(vao.handle, 1);
    glEnableVertexArrayAttrib(vao.handle, 2);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    // Vertex attributes
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
#endif
}

void Mesh::load(const std::vector<VertexPCNT>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(vertices);
    ebo.load(indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    size_t stride = sizeof(VertexPCNT);

#ifdef MODERN_GL
    glVertexArrayVertexBuffer(vao.handle, 0, vbo.handle, 0, stride);
    glVertexArrayElementBuffer(vao.handle, ebo.handle);
    glVertexArrayAttribFormat(vao.handle, 0, 3, GL_FLOAT, GL_FALSE, (void*) (0 * sizeof(float)));
    glVertexArrayAttribFormat(vao.handle, 1, 2, GL_FLOAT, GL_FALSE, (void*) (3 * sizeof(float)));
    glVertexArrayAttribFormat(vao.handle, 2, 3, GL_FLOAT, GL_FALSE, (void*) (5 * sizeof(float)));
    glVertexArrayAttribFormat(vao.handle, 3, 3, GL_FLOAT, GL_FALSE, (void*) (8 * sizeof(float)));
    glEnableVertexArrayAttrib(vao.handle, 0);
    glEnableVertexArrayAttrib(vao.handle, 1);
    glEnableVertexArrayAttrib(vao.handle, 2);
    glEnableVertexArrayAttrib(vao.handle, 3);
#else
    vao.bind();
    vbo.bind();
    ebo.bind();
    // Vertex attributes
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
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

void Mesh::draw(GLuint instances) {
    vao.bind();
    glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0, instances);
}