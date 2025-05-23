#include "vertexarray.hpp"

#include <glad/gl.h>

/////////////////////// RAII behavior ///////////////////////
VertexArray::VertexArray() {
#ifdef MODERN_GL
    glCreateVertexArrays(1, &handle);
#else
    glGenVertexArrays(1, &handle);
#endif
}

VertexArray::VertexArray(VertexArray &&other) noexcept : handle(other.handle) {
    other.handle = 0;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
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