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
    void release();
    void load(std::string vs, std::string fs);
    void attach(Shader shader);
    void attach(std::string filename, GLuint type);
    void link();
    void bind();
    GLuint uniform(std::string name);
    void set(GLuint loc, int value);
    void set(GLuint loc, float value);
    void set(GLuint loc, glm::ivec2& value);
    void set(GLuint loc, glm::vec2& value);
    void set(GLuint loc, glm::ivec3& value);
    void set(GLuint loc, glm::vec3& value);
private:
    GLuint handle;
    std::list<Shader> shaders;
};