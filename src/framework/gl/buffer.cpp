#include "buffer.hpp"

#include <glbinding/gl/gl.h>
using namespace gl;

#include <cassert>
#include <vector>

/////////////////////// RAII behavior ///////////////////////
Buffer::Buffer() {
    glGenBuffers(1, &handle);
    assert(handle);
}

Buffer::Buffer(Buffer&& other) : handle(other.handle) {
    other.handle = 0;
}

Buffer& Buffer::operator=(Buffer&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
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

void Buffer::bind(GLenum type) {
    glBindBuffer(static_cast<GLenum>(type), handle);
}

void Buffer::bind(GLenum type, GLuint index) {
    glBindBufferBase(static_cast<GLenum>(type), index, handle);
}

void Buffer::_load(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage) {
    bind(type);
    glBufferData(static_cast<GLenum>(type), size, data, static_cast<GLenum>(usage));
}

void Buffer::_set(GLenum type, GLsizeiptr size, const GLvoid* data, GLintptr offset) {
    bind(type);
    glBufferSubData(static_cast<GLenum>(type), offset, size, data);
}

void Buffer::allocate(GLenum type, GLsizeiptr size, GLenum usage) {
    bind(type);
    glBufferData(static_cast<GLenum>(type), size, nullptr, static_cast<GLenum>(usage));
}