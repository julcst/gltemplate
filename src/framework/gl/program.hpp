#pragma once

#include "shader.hpp"

#include <glad/glad.h>

#include <string>
#include <list>

/*
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
    void attach(const std::string& filename, GLuint type);
    void link();
    void bind();
    GLuint uniform(const std::string& name);
    void set(GLuint loc, int value);
    void set(GLuint loc, unsigned int value);
    void set(GLuint loc, size_t value);
    void set(GLuint loc, float value);
    void set(GLuint loc, const glm::ivec2& value);
    void set(GLuint loc, const glm::vec2& value);
    void set(GLuint loc, const glm::ivec3& value);
    void set(GLuint loc, const glm::vec3& value);
    void set(GLuint loc, const glm::ivec4& value);
    void set(GLuint loc, const glm::vec4& value);
    void set(GLuint loc, const glm::mat3& value);
    void set(GLuint loc, const glm::mat4& value);
    void set(GLuint loc, const glm::vec4& values, GLuint n);
private:
    GLuint handle;
    std::list<Shader> shaders;
    void release();
};