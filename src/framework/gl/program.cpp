#include "program.hpp"

#include "shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <string>
#include <list>
#include <iostream>

using namespace glm;

/////////////////////// RAII behavior ///////////////////////
Program::Program() {
    handle = glCreateProgram();
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
        throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
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

void Program::set(GLuint loc, float value) {
    glProgramUniform1f(handle, loc, value);
}

void Program::set(GLuint loc, ivec2& value) {
    glProgramUniform2iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, vec2& value) {
    glProgramUniform2fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, ivec3& value) {
    glProgramUniform3iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, vec3& value) {
    glProgramUniform3fv(handle, loc, 1, value_ptr(value));
}
