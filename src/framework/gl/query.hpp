#pragma once

#include <glbinding/gl/gl.h>

using namespace gl;

/**
 * RAII wrapper for OpenGL query object
 */
class Query {
   public:
    
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