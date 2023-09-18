#include "shader.hpp"

#include "config.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>

using namespace glm;

const std::regex includeRegex("#include \"([^\"]+)\"");

std::string readFile(std::string filename) {
    std::ifstream stream(SHADER_DIR + filename);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string readShader(std::string filename) {
    std::string source = readFile(filename);
    std::smatch match;
    while (std::regex_search(source, match, includeRegex)) {
        std::string include = readFile(match[1]);
        source = match.prefix().str() + include + match.suffix().str();
    }
    return source;
}

Shader::Shader(std::string filename, unsigned int type) {
    std::string source = readShader(filename);
    const char* sourcePtr = source.c_str();
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourcePtr, NULL);
    compile();
}

Shader::~Shader() {
    glDeleteShader(shader);
}

void Shader::compile() {
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
    }
}

unsigned int Shader::getHandle() {
    return shader;
}

ShaderProgram::ShaderProgram() : program(0) {}

ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader) {
    program = glCreateProgram();
    attach(Shader(vertexShader, GL_VERTEX_SHADER));
    attach(Shader(fragmentShader, GL_FRAGMENT_SHADER));
    link();
}

ShaderProgram::~ShaderProgram() {
    //glDeleteProgram(program);
}

void ShaderProgram::attach(Shader shader) {
    glAttachShader(program, shader.getHandle());
}

void ShaderProgram::link() {
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
    }
}

void ShaderProgram::bind() {
    glUseProgram(program);
}

unsigned int ShaderProgram::uniform(std::string name) {
    return glGetUniformLocation(program, name.c_str());
}

void ShaderProgram::set(unsigned int loc, int value) {
    glUniform1i(loc, value);
}

void ShaderProgram::set(unsigned int loc, float value) {
    glUniform1f(loc, value);
}

void ShaderProgram::set(unsigned int loc, ivec2 value) {
    glUniform1iv(loc, 2, value_ptr(value));
}

void ShaderProgram::set(unsigned int loc, vec2 value) {
    glUniform1fv(loc, 2, value_ptr(value));
}

void ShaderProgram::set(unsigned int loc, ivec3 value) {
    glUniform1iv(loc, 3, value_ptr(value));
}

void ShaderProgram::set(unsigned int loc, vec3 value) {
    glUniform1fv(loc, 3, value_ptr(value));
}