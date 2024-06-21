#include "buffer.hpp"

#include <cassert>
#include <vector>

#include <glbinding/gl/gl.h>

using namespace gl;

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
    glBindBuffer(type, handle);
}

void Buffer::bind(GLenum type, GLuint index) {
    glBindBufferBase(type, index, handle);
}

void Buffer::_load(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage) {
    bind(type);
    glBufferData(type, size, data, usage);
}

void Buffer::_set(GLenum type, GLsizeiptr size, const GLvoid* data, GLintptr offset) {
    bind(type);
    glBufferSubData(type, offset, size, data);
}

void Buffer::allocate(GLenum type, GLsizeiptr size, GLenum usage) {
    bind(type);
    glBufferData(type, size, nullptr, usage);
}