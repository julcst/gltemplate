#pragma once

#include <glbinding/gl/gl.h>
using namespace gl;

#include <vector>

/**
 * RAII wrapper for OpenGL buffer
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
    void bind(GLenum type);
    void bind(GLenum type, GLuint index);

    void _load(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);
    template <typename T>
    void load(GLenum type, const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW);
    template <typename T>
    void load(GLenum type, const T& data, GLenum usage = GL_STATIC_DRAW);

    void _set(GLenum type, GLsizeiptr size, const GLvoid* data, GLintptr offset);
    template <typename T>
    void set(GLenum type, const std::vector<T>& data, unsigned int offset = 0);
    template <typename T>
    void set(GLenum type, const T& data, unsigned int offset = 0);

    void allocate(GLenum type, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    GLuint handle;

   private:
    void release();
};

template <typename T>
inline void Buffer::load(GLenum type, const std::vector<T>& data, GLenum usage) {
    _load(type, sizeof(T) * data.size(), data.data(), usage);
}

template <typename T>
inline void Buffer::load(GLenum type, const T& data, GLenum usage) {
    _load(type, sizeof(T), &data, usage);
}

template <typename T>
inline void Buffer::set(GLenum type, const std::vector<T>& data, unsigned int offset) {
    _set(type, sizeof(T) * data.size(), data.data(), offset);
}

template <typename T>
inline void Buffer::set(GLenum type, const T& data, unsigned int offset) {
    _set(type, sizeof(T), &data, offset);
}