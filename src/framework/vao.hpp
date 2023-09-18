#pragma once

/*
 * RAII wrapper for OpenGL Vertex Array Object
 */
class VAO {
public:
    VAO();
    VAO(float vertices[], unsigned int nVertices, unsigned int indices[], unsigned int nTriangle);
    ~VAO();
    void bind();
private:
    unsigned int handle, VBO, EBO;
};