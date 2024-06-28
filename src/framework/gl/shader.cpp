#include "shader.hpp"

#include <glbinding/gl/gl.h>
using namespace gl;

#include <cassert>
#include <regex>
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <filesystem>

#include "framework/common.hpp"
#include "framework/context.hpp"

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
    if (handle) glDeleteShader(handle);
}
/////////////////////////////////////////////////////////////

const std::regex includeRegex("(?:^|\n)#include \"([^\"]+)\"");

std::string readShader(const std::filesystem::path& filepath, std::unordered_set<std::filesystem::path>& included) {
    std::string source = Common::readFile(filepath);
    std::smatch match;
    while (std::regex_search(source, match, includeRegex)) {
        std::filesystem::path includePath = filepath.parent_path() / match[1].str();
        if (included.insert(includePath).second) {
            try {
                std::string include = readShader(includePath, included);
                source = match.prefix().str() + include + match.suffix().str();
            } catch (const std::runtime_error& e) {
                throw std::runtime_error("Error including \"" + match[1].str() + "\" in \"" + filepath.string().c_str() + "\": " + e.what());
            }
        } else {
            source = match.prefix().str() + match.suffix().str();
        }
    }
    return source;
}

void Shader::load(const std::filesystem::path& filepath) {
    std::unordered_set<std::filesystem::path> included;
    std::string source = readShader(filepath, included);
#ifdef COMPOSE_SHADERS
    Common::writeToFile(source, Context::COMPOSED_SHADER_DIR / filepath.filepath);
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