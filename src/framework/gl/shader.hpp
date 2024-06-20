#pragma once

#include <glbinding/gl/gl.h>
using namespace gl;

#include <glm/glm.hpp>

#include <filesystem>

class Shader {
   public:
    enum class Type: uint {
        VERTEX_SHADER = static_cast<uint>(GL_VERTEX_SHADER),
        FRAGMENT_SHADER = static_cast<uint>(GL_FRAGMENT_SHADER),
    };
    
    Shader(GLenum type);
    // Disable copying
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    // Implement moving
    Shader(Shader&& other);
    Shader& operator=(Shader&& other);
    ~Shader();
    void load(const std::filesystem::path& filepath);
    void compile();

    GLuint handle;

   private:
    void release();
};