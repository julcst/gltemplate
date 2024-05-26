#include "program.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <vector>
#include <stdexcept>
#include <string>

#include "shader.hpp"

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
    if (handle) glDeleteProgram(handle);
}
/////////////////////////////////////////////////////////////

void Program::load(const std::string& vs, const std::string& fs) {
    attach(vs, Shader::Type::VERTEX_SHADER);
    attach(fs, Shader::Type::FRAGMENT_SHADER);
    link();
}

void Program::attach(Shader shader) {
    glAttachShader(handle, shader.handle);
    shaders.push_back(std::move(shader));
}

void Program::attach(const std::string& filename, Shader::Type type) {
    Shader shader(type);
    shader.load(filename);
    glAttachShader(handle, shader.handle);
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

GLuint Program::uniform(const std::string& name) {
    return glGetUniformLocation(handle, name.c_str());
}

void Program::bindUBO(const std::string& loc, GLuint index) {
    GLuint i = glGetUniformBlockIndex(handle, loc.c_str());
    glUniformBlockBinding(handle, i, index);
}

void Program::bindTextureUnit(const std::string& loc, GLint index) {
    set(uniform(loc), index);
}

void Program::set(GLuint loc, GLint value) {
    glProgramUniform1i(handle, loc, value);
}

void Program::set(GLuint loc, GLuint value) {
    glProgramUniform1ui(handle, loc, value);
}

void Program::set(GLuint loc, GLfloat value) {
    glProgramUniform1f(handle, loc, value);
}

void Program::set(GLuint loc, const std::vector<float>& values) {
    glProgramUniform3fv(handle, loc, values.size(), values.data());
}

void Program::set(GLuint loc, const ivec2& value) {
    glProgramUniform2iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const vec2& value) {
    glProgramUniform2fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const std::vector<vec2>& values) {
    glProgramUniform2fv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLuint loc, const ivec3& value) {
    glProgramUniform3iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const vec3& value) {
    glProgramUniform3fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const std::vector<vec3>& values) {
    glProgramUniform3fv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLuint loc, const ivec4& value) {
    glProgramUniform4iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const vec4& value) {
    glProgramUniform4fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLuint loc, const std::vector<vec4>& values) {
    glProgramUniform4fv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLuint loc, const mat3& value) {
    glProgramUniformMatrix3fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}

void Program::set(GLuint loc, const mat4& value) {
    glProgramUniformMatrix4fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}