#include "program.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <stdexcept>
#include <string>
#include <array>

#include "shader.hpp"

/////////////////////// RAII behavior ///////////////////////
Program::Program() : handle(glCreateProgram()) {}

Program::Program(Program &&other) noexcept : handle(other.handle) {
    other.handle = 0;
}

Program &Program::operator=(Program &&other) noexcept {
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

void Program::load(const std::filesystem::path& vs, const std::filesystem::path& fs) {
    attach<GL_VERTEX_SHADER>(vs);
    attach<GL_FRAGMENT_SHADER>(fs);
    link();
}

void Program::loadSource(const std::string& vs, const std::string& fs) {
    attachSource<GL_VERTEX_SHADER>(vs);
    attachSource<GL_FRAGMENT_SHADER>(fs);
    link();
}

void Program::attach(GLuint shader) {
    glAttachShader(handle, shader);
}

void Program::link() {
    glLinkProgram(handle);
    GLint success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (!success) {
        std::array<char, 512> infoLog;
        glGetProgramInfoLog(handle, infoLog.size(), nullptr, infoLog.data());
        throw std::runtime_error("Program linking failed: " + std::string(infoLog.data()));
    }
}

void Program::use() {
    glUseProgram(handle);
}

GLint Program::uniform(const std::string& name) {
    return glGetUniformLocation(handle, name.c_str());
}

void Program::bindUBO(const std::string& loc, GLuint index) {
    GLuint i = glGetUniformBlockIndex(handle, loc.c_str());
    glUniformBlockBinding(handle, i, index);
}

void Program::bindTextureUnit(const std::string& loc, GLint index) {
    set(uniform(loc), index);
}

void Program::set(GLint loc, GLint value) {
    glProgramUniform1i(handle, loc, value);
}

void Program::set(GLint loc, const std::vector<GLint>& values) {
    glProgramUniform1iv(handle, loc, values.size(), values.data());
}

void Program::set(GLint loc, GLuint value) {
    glProgramUniform1ui(handle, loc, value);
}

void Program::set(GLint loc, const std::vector<GLuint>& values) {
    glProgramUniform1uiv(handle, loc, values.size(), values.data());
}

void Program::set(GLint loc, GLfloat value) {
    glProgramUniform1f(handle, loc, value);
}

void Program::set(GLint loc, const std::vector<float>& values) {
    glProgramUniform1fv(handle, loc, values.size(), values.data());
}

void Program::set(GLint loc, const ivec2& value) {
    glProgramUniform2iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<ivec2>& values) {
    glProgramUniform2iv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLint loc, const vec2& value) {
    glProgramUniform2fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<vec2>& values) {
    glProgramUniform2fv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLint loc, const ivec3& value) {
    glProgramUniform3iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<ivec3>& values) {
    glProgramUniform3iv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLint loc, const vec3& value) {
    glProgramUniform3fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<vec3>& values) {
    glProgramUniform3fv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLint loc, const ivec4& value) {
    glProgramUniform4iv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<ivec4>& values) {
    glProgramUniform4iv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLint loc, const vec4& value) {
    glProgramUniform4fv(handle, loc, 1, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<vec4>& values) {
    glProgramUniform4fv(handle, loc, values.size(), value_ptr(values[0]));
}

void Program::set(GLint loc, const mat2& value) {
    glProgramUniformMatrix2fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<mat2>& values) {
    glProgramUniformMatrix2fv(handle, loc, values.size(), GL_FALSE, value_ptr(values[0]));
}

void Program::set(GLint loc, const mat3& value) {
    glProgramUniformMatrix3fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<mat3>& values) {
    glProgramUniformMatrix3fv(handle, loc, values.size(), GL_FALSE, value_ptr(values[0]));
}

void Program::set(GLint loc, const mat4& value) {
    glProgramUniformMatrix4fv(handle, loc, 1, GL_FALSE, value_ptr(value));
}

void Program::set(GLint loc, const std::vector<mat4>& values) {
    glProgramUniformMatrix4fv(handle, loc, values.size(), GL_FALSE, value_ptr(values[0]));
}