#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader {
public:
    Shader(GLenum type);
    // Disable copying
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    // Implement moving
    Shader(Shader&& other);
    Shader& operator=(Shader&& other);
    ~Shader();
    void load(const std::string& filename);
    void compile();
    GLuint getHandle();
private:
    GLuint handle;
    void release();
};