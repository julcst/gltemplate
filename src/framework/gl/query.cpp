#include "query.hpp"

#include <cassert>

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

void Query::begin(Type type) {
    glBeginQuery(static_cast<GLenum>(type), handle);
}

GLuint Query::end(Type type) {
    glEndQuery(static_cast<GLenum>(type));
    GLuint result;
    glGetQueryObjectuiv(handle, GL_QUERY_RESULT, &result);
    return result;
}