#include "mesh.hpp"

#include <glad/glad.h>

#include <vector>

void Mesh::load(std::vector<float> vertices, std::vector<unsigned int> indices) {
    // Load data into buffers
    numVertices = vertices.size();
    numIndices = indices.size();
    vbo.load(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices.data());
    ebo.load(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices.data());

    // Bind buffers to VAO
    bind();
    vbo.bind(GL_ARRAY_BUFFER);
    ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    unbind();
}

void Mesh::draw() {
    bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    unbind();
}