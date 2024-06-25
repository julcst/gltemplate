#pragma once

#include <glbinding/gl/gl.h>

#include <string>
#include <filesystem>

#include "buffer.hpp"
#include "shader.hpp"

using namespace gl;

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
    void load(const std::filesystem::path& vs, const std::filesystem::path& fs);
    void attach(const std::filesystem::path& filepath, GLenum type);
    void attach(const Shader& shader);
    void attach(GLuint shader);
    void link();
    void use();
    GLuint uniform(const std::string& name);
    void bindUBO(const std::string& loc, GLuint index);
    void bindTextureUnit(const std::string& loc, GLint index);

    // Uniform setters with location
    void set(GLuint loc, const GLint value);
    void set(GLuint loc, const std::vector<GLint>& values);
    void set(GLuint loc, const GLuint value);
    void set(GLuint loc, const std::vector<GLuint>& values);
    void set(GLuint loc, const GLfloat value);
    void set(GLuint loc, const std::vector<GLfloat>& values);
    void set(GLuint loc, const glm::ivec2& value);
    void set(GLuint loc, const std::vector<glm::ivec2>& values);
    void set(GLuint loc, const glm::vec2& value);
    void set(GLuint loc, const std::vector<glm::vec2>& values);
    void set(GLuint loc, const glm::ivec3& value);
    void set(GLuint loc, const std::vector<glm::ivec3>& values);
    void set(GLuint loc, const glm::vec3& value);
    void set(GLuint loc, const std::vector<glm::vec3>& values);
    void set(GLuint loc, const glm::ivec4& value);
    void set(GLuint loc, const std::vector<glm::ivec4>& values);
    void set(GLuint loc, const glm::vec4& value);
    void set(GLuint loc, const std::vector<glm::vec4>& values);
    void set(GLuint loc, const glm::mat2& value);
    void set(GLuint loc, const std::vector<glm::mat2>& values);
    void set(GLuint loc, const glm::mat3& value);
    void set(GLuint loc, const std::vector<glm::mat3>& values);
    void set(GLuint loc, const glm::mat4& value);
    void set(GLuint loc, const std::vector<glm::mat4>& values);

    // Uniform setters with name
    template <typename T>
    void set(const std::string& loc, const T& value);

    GLuint handle;

   private:
    void release();
};

template <typename T>
inline void Program::set(const std::string& loc, const T& value) {
    Program::set(Program::uniform(loc), value);
}