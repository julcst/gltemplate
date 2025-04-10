#include "query.hpp"

#include <glad/glad.h>

/////////////////////// RAII behavior ///////////////////////
Query::Query() {
    glGenQueries(1, &handle);
}

Query::Query(Query &&other) noexcept : handle(other.handle) {
    other.handle = 0;
}

Query &Query::operator=(Query &&other) noexcept {
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