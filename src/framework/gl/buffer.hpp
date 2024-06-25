#pragma once

#include <glbinding/gl/gl.h>
using namespace gl;

#include <vector>

/**
 * RAII wrapper for OpenGL buffer
 */
class Buffer {
   public:
    
    Buffer(GLenum type);
    // Disable copying
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    // Implement moving
    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);
    ~Buffer();
    void bind();
    void bind(GLuint index);

    void _load(GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);
    template <typename T>
    void load(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW);
    template <typename T>
    void load(const T& data, GLenum usage = GL_STATIC_DRAW);

    void _set(GLsizeiptr size, const GLvoid* data, GLintptr offset);
    template <typename T>
    void set(const std::vector<T>& data, unsigned int offset = 0);
    template <typename T>
    void set(const T& data, unsigned int offset = 0);

    void allocate(GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    GLuint handle;
    GLenum type;

   private:
    void release();
};

template <typename T>
inline void Buffer::load(const std::vector<T>& data, GLenum usage) {
    _load(sizeof(T) * data.size(), data.data(), usage);
}

template <typename T>
inline void Buffer::load(const T& data, GLenum usage) {
    _load(sizeof(T), &data, usage);
}

template <typename T>
inline void Buffer::set(const std::vector<T>& data, unsigned int offset) {
    _set(sizeof(T) * data.size(), data.data(), offset);
}

template <typename T>
inline void Buffer::set(const T& data, unsigned int offset) {
    _set(sizeof(T), &data, offset);
}