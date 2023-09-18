#pragma once

#include <glad/glad.h>

/*
 * RAII wrapper for OpenGL vertex buffer
 */
class Buffer {
public:
    Buffer();
    // Disable copying
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    // Implement moving
    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);
    ~Buffer();
    void release();
    void bind(GLenum type);
    void load(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);
private:
    GLuint handle;
};