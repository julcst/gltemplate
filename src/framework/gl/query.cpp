#include "query.hpp"

#include <cassert>

#include <glbinding/gl46core/gl.h>

using namespace gl46core;

/////////////////////// RAII behavior ///////////////////////
Query::Query() {
    glGenQueries(1, &handle);
    assert(handle);
}

Query::Query(Query&& other) : handle(other.handle) {
    other.handle = 0;
}

Query& Query::operator=(Query&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

Query::~Query() {
    release();
}

void Query::release() {
    if (handle) glDeleteQueries(1, &handle);
}
/////////////////////////////////////////////////////////////

void Query::begin(GLenum target) {
    glBeginQuery(target, handle);
}

GLuint Query::end(GLenum target) {
    glEndQuery(target);
    GLuint result;
    glGetQueryObjectuiv(handle, GL_QUERY_RESULT, &result);
    return result;
}