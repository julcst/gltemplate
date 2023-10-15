#include "program.hpp"

#include "shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <stdexcept>
#include <string>
#include <list>
#include <iostream>

using namespace glm;

/////////////////////// RAII behavior ///////////////////////
Program::Program() {
    handle = glCreateProgram();
    assert(handle);
}

Program::Program(Program&& other) : handle(other.handle) {
    other.handle = 0;
}

Program& Program::operator=(Program&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

Program::~Program() {
    release();
}

void Program::release() {
    if(handle) glDeleteProgram(handle);
}
/////////////////////////////////////////////////////////////

void Program::load(std::string vs, std::string fs) {
    attach(vs, GL_VERTEX_SHADER);
    attach(fs, GL_FRAGMENT_SHADER);
    link();
}

void Program::attach(Shader shader) {
    glAttachShader(handle, shader.getHandle());
    shaders.push_back(std::move(shader));
}

void Program::attach(std::string filename, GLuint type) {
    Shader shader(type);
    shader.load(filename);
    glAttachShader(handle, shader.getHandle());
    shaders.push_back(std::move(shader));
}

void Program::link() {
    glLinkProgram(handle);
    int success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(handle, 512, NULL, infoLog);
        throw std::runtime_error("Program linking failed: " + std::string(infoLog));
    }
}

void Program::bind() {
    glUseProgram(handle);
}

GLuint Program::uniform(std::string name) {
    return glGetUniformLocation(handle, name.c_str());
}

void Program::set(GLuint loc, int value) {
    glProgramUniform1i(handle, loc, value);
}

void Program::set(GLuint loc, unsigned int value) {
    glProgramUniform1ui(handle, loc, value);
}

void Program::set(GLuint loc, float value) {
    glProgramUniform1f(handle, loc, value);
}

void Program::set(GLuint loc, const ivec2& value) {
    glProgramUniform2iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const vec2& value) {
    glProgramUniform2fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const ivec3& value) {
    glProgramUniform3iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const vec3& value) {
    glProgramUniform3fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const ivec4& value) {
    glProgramUniform4iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const vec4& value) {
    glProgramUniform4fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const mat3& value) {
    glProgramUniformMatrix3fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}

void Program::set(GLuint loc, const mat4& value) {
    glProgramUniformMatrix4fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}

void Program::set(GLuint loc, const vec4& values, GLuint n) {
    glProgramUniform4fv(handle, loc, n, value_ptr(values));
}