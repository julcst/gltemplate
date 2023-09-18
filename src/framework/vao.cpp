#include "vao.hpp"

#include <glad/glad.h>

#include <stdexcept>
#include <iostream>

unsigned int makeBuffer(unsigned int type, unsigned int size, void* data) {
    unsigned int handle;
    glGenBuffers(1, &handle);
    if (!data) throw std::runtime_error("Buffer data is null");
    glBindBuffer(type, handle);
    glBufferData(type, size, data, GL_STATIC_DRAW);
    return handle;
}

VAO::VAO() : handle(0) {
    //glGenVertexArrays(1, &handle);
    std::cout << "VAO created: " << handle << std::endl;
}

VAO::VAO(float vertices[], unsigned int nVertices, unsigned int indices[], unsigned int nTriangle) {
    glGenVertexArrays(1, &handle);
    if (!handle) throw std::runtime_error("Failed to create VAO");
    std::cout << "VAO created: " << handle << std::endl;
    // Bind data
    glBindVertexArray(handle);
    makeBuffer(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), vertices);
    makeBuffer(GL_ELEMENT_ARRAY_BUFFER, nTriangle * 3 * sizeof(unsigned int), indices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Unbind
    glBindVertexArray(0);
}


VAO::~VAO() {
    std::cout << "VAO deleted: " << handle << std::endl;
    /*glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &handle);*/
}

void VAO::bind() {
    glBindVertexArray(handle);
}