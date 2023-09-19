#include "buffer.hpp"

#include <glad/glad.h>

#include <cassert>

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
    if(handle) glDeleteBuffers(1, &handle);
}
/////////////////////////////////////////////////////////////

void Buffer::bind(GLenum type) {
    glBindBuffer(type, handle);
}

void Buffer::load(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage) {
    bind(type);
    glBufferData(type, size, data, usage);
}