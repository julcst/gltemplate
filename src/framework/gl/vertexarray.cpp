#include "vertexarray.hpp"

#include <glad/glad.h>

#include <cassert>

/////////////////////// RAII behavior ///////////////////////
VertexArray::VertexArray() {
    glGenVertexArrays(1, &handle);
    assert(handle);
}

VertexArray::VertexArray(VertexArray&& other) : handle(other.handle) {
    other.handle = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

VertexArray::~VertexArray() {
    release();
}

void VertexArray::release() {
    if (handle) glDeleteVertexArrays(1, &handle);
}
/////////////////////////////////////////////////////////////

void VertexArray::bind() {
    glBindVertexArray(handle);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}