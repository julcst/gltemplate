#pragma once

#include <glad/glad.h>

/*
 * RAII wrapper for OpenGL vertex array
 */
class VertexArray {
public:
    VertexArray();
    // Disable copying
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    // Implement moving
    VertexArray(VertexArray&& other);
    VertexArray& operator=(VertexArray&& other);
    virtual ~VertexArray();
    void bind();
    void unbind();
    virtual void draw();
private:
    GLuint handle;
    void release();
};