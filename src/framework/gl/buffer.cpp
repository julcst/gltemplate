#include "buffer.hpp"

#include <cassert>
#include <vector>

#include <glbinding/gl/gl.h>

using namespace gl;

/////////////////////// RAII behavior ///////////////////////
Buffer::Buffer(GLenum type) : type(type) {
#ifdef MODERN_GL
    glCreateBuffers(1, &handle);
#else
    glGenBuffers(1, &handle);
#endif
    assert(handle);
}

Buffer::Buffer(Buffer&& other) : handle(other.handle), type(other.type) {
    other.handle = 0;
}

Buffer& Buffer::operator=(Buffer&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        type = other.type;
        other.handle = 0;
    }
    return *this;
}

Buffer::~Buffer() {
    release();
}

void Buffer::release() {
    if (handle) glDeleteBuffers(1, &handle);
}
/////////////////////////////////////////////////////////////

void Buffer::bind() {
    glBindBuffer(type, handle);
}

void Buffer::bind(GLuint index) {
    glBindBufferBase(type, index, handle);
}

void Buffer::_load(GLsizeiptr size, const GLvoid* data, GLenum usage) {
#ifdef MODERN_GL
    glNamedBufferData(handle, size, data, usage);
#else
    bind();
    glBufferData(type, size, data, usage);
#endif
}

void Buffer::_set(GLsizeiptr size, const GLvoid* data, GLintptr offset) {
#ifdef MODERN_GL
    glNamedBufferSubData(handle, offset, size, data);
#else
    bind();
    glBufferSubData(type, offset, size, data);
#endif
}

void Buffer::allocate(GLsizeiptr size, GLenum usage) {
#ifdef MODERN_GL
    glNamedBufferData(handle, size, nullptr, usage);
#else
    bind();
    glBufferData(type, size, nullptr, usage);
#endif
}