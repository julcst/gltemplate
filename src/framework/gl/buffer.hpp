#pragma once

#include <glbinding/gl/gl.h>
using namespace gl;

#include <vector>

/**
 * RAII wrapper for OpenGL buffer
 */
class Buffer {
   public:
    enum class Type: uint {
        ARRAY_BUFFER = static_cast<uint>(GL_ARRAY_BUFFER),
        UNIFORM_BUFFER = static_cast<uint>(GL_UNIFORM_BUFFER),
        INDEX_BUFFER = static_cast<uint>(GL_ELEMENT_ARRAY_BUFFER),
    };
    enum class Usage: uint {
        STATIC_DRAW = static_cast<uint>(GL_STATIC_DRAW),
        DYNAMIC_DRAW = static_cast<uint>(GL_DYNAMIC_DRAW),
    };
    
    Buffer();
    // Disable copying
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    // Implement moving
    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);
    ~Buffer();
    void bind(Type type);
    void bind(Type type, GLuint index);

    void _load(Type type, GLsizeiptr size, const GLvoid* data, Usage usage = Usage::STATIC_DRAW);
    template <typename T>
    void load(Type type, const std::vector<T>& data, Usage usage = Usage::STATIC_DRAW);
    template <typename T>
    void load(Type type, const T& data, Usage usage = Usage::STATIC_DRAW);

    void _set(Type type, GLsizeiptr size, const GLvoid* data, GLintptr offset);
    template <typename T>
    void set(Type type, const std::vector<T>& data, unsigned int offset = 0);
    template <typename T>
    void set(Type type, const T& data, unsigned int offset = 0);

    void allocate(Type type, GLsizeiptr size, Usage usage = Usage::STATIC_DRAW);

    GLuint handle;

   private:
    void release();
};

template <typename T>
inline void Buffer::load(Type type, const std::vector<T>& data, Usage usage) {
    _load(type, sizeof(T) * data.size(), data.data(), usage);
}

template <typename T>
inline void Buffer::load(Type type, const T& data, Usage usage) {
    _load(type, sizeof(T), &data, usage);
}

template <typename T>
inline void Buffer::set(Type type, const std::vector<T>& data, unsigned int offset) {
    _set(type, sizeof(T) * data.size(), data.data(), offset);
}

template <typename T>
inline void Buffer::set(Type type, const T& data, unsigned int offset) {
    _set(type, sizeof(T), &data, offset);
}