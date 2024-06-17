#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>

class Shader {
   public:
    enum class Type {
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    };
    
    Shader(Type type);
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