#include "vertexarray.hpp"

#include <glbinding/gl/gl.h>
using namespace gl;

#include <cassert>

/////////////////////// RAII behavior ///////////////////////
VertexArray::VertexArray() {
#ifdef MODERN_GL
    glCreateVertexArrays(1, &handle);
#else
    glGenVertexArrays(1, &handle);
#endif
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