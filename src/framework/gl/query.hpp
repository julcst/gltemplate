#pragma once

#include <glbinding/gl/gl.h>
using namespace gl;

/**
 * RAII wrapper for OpenGL query object
 */
class Query {
   public:
    enum class Type: uint {
        TIME_ELAPSED = static_cast<uint>(GL_TIME_ELAPSED),
    };
    
    Query();
    // Disable copying
    Query(const Query&) = delete;
    Query& operator=(const Query&) = delete;
    // Implement moving
    Query(Query&& other);
    Query& operator=(Query&& other);
    ~Query();
    void begin(GLenum type);
    GLuint end(GLenum type);

    GLuint handle;

   private:
    void release();
};