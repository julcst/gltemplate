#pragma once

#include <glad/glad.h>

/**
 * RAII wrapper for OpenGL query object
 */
class Query {
   public:
    enum class Type {
        TIME_ELAPSED = GL_TIME_ELAPSED,
    };
    
    Query();
    // Disable copying
    Query(const Query&) = delete;
    Query& operator=(const Query&) = delete;
    // Implement moving
    Query(Query&& other);
    Query& operator=(Query&& other);
    ~Query();
    void begin(Type type);
    GLuint end(Type type);

    GLuint handle;

   private:
    void release();
};