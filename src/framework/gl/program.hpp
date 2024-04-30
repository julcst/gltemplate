#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

#include "buffer.hpp"
#include "shader.hpp"

/**
 * RAII wrapper for OpenGL program
 */
class Program {
   public:
    Program();
    // Disable copying
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    // Implement moving
    Program(Program&& other);
    Program& operator=(Program&& other);
    ~Program();
    void load(const std::string& vs, const std::string& fs);
    void attach(Shader shader);
    void attach(const std::string& filename, Shader::Type type);
    void link();
    void bind();
    GLuint uniform(const std::string& name);
    void bindUBO(const std::string& loc, GLuint index);
    void bindTextureUnit(const std::string& loc, GLint index);
    void set(GLuint loc, GLint value);
    void set(GLuint loc, GLuint value);
    void set(GLuint loc, GLfloat value);
    void set(GLuint loc, const glm::ivec2& value);
    void set(GLuint loc, const glm::vec2& value);
    void set(GLuint loc, const glm::ivec3& value);
    void set(GLuint loc, const glm::vec3& value);
    void set(GLuint loc, const glm::ivec4& value);
    void set(GLuint loc, const glm::vec4& value);
    void set(GLuint loc, const glm::mat3& value);
    void set(GLuint loc, const glm::mat4& value);
    template <typename T>
    void set(const std::string& loc, const T& value);

    GLuint handle;
    std::vector<Shader> shaders;

   private:
    void release();
};

template <typename T>
inline void Program::set(const std::string& loc, const T& value) {
    Program::set(Program::uniform(loc), value);
}