#include "shader.hpp"

#include <glad/glad.h>

#include <cassert>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>

#include "common.hpp"
#include "config.hpp"

/////////////////////// RAII behavior ///////////////////////
Shader::Shader(Type type) {
    handle = glCreateShader(static_cast<GLenum>(type));
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
    if (handle) glDeleteShader(handle);
}
/////////////////////////////////////////////////////////////

const std::regex includeRegex("(?:^|\n)#include \"([^\"]+)\"");

std::string readShader(const std::string& filename, std::set<std::string>& included) {
    std::string source = Common::readFile(Config::SHADER_DIR + filename);
    std::smatch match;
    while (std::regex_search(source, match, includeRegex)) {
        if (included.find(match[1].str()) == included.end()) {
            included.insert(match[1].str());
            try {
                std::string include = readShader(match[1].str(), included);
                source = match.prefix().str() + include + match.suffix().str();
            } catch (const std::runtime_error& e) {
                throw std::runtime_error("Error including \"" + match[1].str() + "\" in \"" + filename + "\": " + e.what());
            }
        } else {
            source = match.prefix().str() + match.suffix().str();
        }
    }
    return source;
}

void Shader::load(const std::string& filename) {
    std::set<std::string> included;
    std::string source = readShader(filename, included);
#ifdef COMPOSE_SHADERS
    Common::writeToFile(source, Config::COMPOSED_SHADER_DIR + filename);
#endif
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