#include "shader.hpp"

#include "config.hpp"

#include <glad/glad.h>

#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <set>

/////////////////////// RAII behavior ///////////////////////
Shader::Shader(GLenum type) {
    handle = glCreateShader(type);
    assert(handle);
}

Shader::Shader(Shader&& other) : handle(other.handle) {
    other.handle = 0;
}

Shader& Shader::operator=(Shader&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

Shader::~Shader() {
    release();
}

void Shader::release() {
    if(handle) glDeleteShader(handle);
}
/////////////////////////////////////////////////////////////

const std::regex includeRegex("#include \"([^\"]+)\"");

std::string readFile(std::string path) {
    std::ifstream stream(path);
    if(!stream.is_open()) throw std::runtime_error("Could not open file: " + path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string readShader(std::string filename, std::set<std::string>& included) {
    std::string source = readFile(SHADER_DIR + filename);
    std::smatch match;
    while (std::regex_search(source, match, includeRegex)) {
        if (included.find(match[1].str()) == included.end()) {
            included.insert(match[1].str());
            std::string include = readShader(match[1].str(), included);
            source = match.prefix().str() + include + match.suffix().str();
        } else {
            source = match.prefix().str() + match.suffix().str();
        }
    }
    return source;
}

void Shader::load(std::string filename) {
    std::set<std::string> included;
    std::string source = readShader(filename, included);
    const char* sourcePtr = source.c_str();
    glShaderSource(handle, 1, &sourcePtr, NULL);
    compile();
}

void Shader::compile() {
    glCompileShader(handle);
    int success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(handle, 512, NULL, infoLog);
        throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
    }
}

GLuint Shader::getHandle() {
    return handle;
}