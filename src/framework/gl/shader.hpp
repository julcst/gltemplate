#pragma once

#include <glad/glad.h>

#include <regex>
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <array>

#include "framework/common.hpp"
#include "framework/context.hpp"

/**
 * @file shader.hpp
 * @brief Defines a Shader class wrapper around the OpenGL shader object.
 */

/**
 * @class Shader
 * @brief RAII wrapper for OpenGL shader with helper functions for loading and compiling shaders.
 * See https://www.khronos.org/opengl/wiki/Shader_Compilation for more information.
 * @tparam target The type of shader, e.g. `GL_VERTEX_SHADER`, `GL_FRAGMENT_SHADER`, `GL_COMPUTE_SHADER`.
 */
template <GLenum type>
class Shader {
   public:
    /**
     * @brief Constructs a Shader object with the specified target.
     * The identifier of the OpenGL object is stored in `Shader::handle`.
     */
    Shader();

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Shader(const Shader&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Shader& operator=(const Shader&) = delete;
    
    /**
     * @brief Move constructor, invalidates the other Shader object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Shader object to move from.
     */
    Shader(Shader &&other) noexcept;

    /**
     * @brief Move assignment operator, deletes the current Texture object and invalidates the other Texture object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Texture object to move from.
     * @return A reference to the moved Texture object.
     */
    Shader &operator=(Shader &&other) noexcept;

    /**
     * @brief Destructor, frees the OpenGL object.
     */
    ~Shader();

    /**
     * @brief Loads and compiles the shader source code from a file.
     * This function will also replace `#include` directives with the content of the included file.
     * It uses `std::unordered_set` to prevent infinite recursion when including files.
     * Files are included relative to the file they are included in.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param filepath The path to the file containing the shader source code.
     */
    void load(const std::filesystem::path& filepath);

    /**
     * @brief Loads and compiles shader source code.
     * @throw `std::runtime_error` when the shader could not be compiled.
     * @param source The shader source code.
     */
    void loadSource(const std::string& source);

    /**
     * @brief Compiles the shader.
     * @throw `std::runtime_error` when the shader could not be compiled.
     */
    void compile();

    /**
     * @brief The unique handle that identifies the shader object on the GPU.
     */
    GLuint handle;

   private:

    /**
     * @brief Frees the OpenGL object.
     */
    void release();
};

/////////////////////// RAII behavior ///////////////////////
template <GLenum type>
Shader<type>::Shader() : handle(glCreateShader(type)) {}

template <GLenum type>
Shader<type>::Shader(Shader<type> &&other) noexcept : handle(other.handle) {
    other.handle = 0;
}

template <GLenum type>
Shader<type> &Shader<type>::operator=(Shader<type> &&other) noexcept {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

template <GLenum type>
Shader<type>::~Shader() {
    release();
}

template <GLenum type>
void Shader<type>::release() {
    if (handle) glDeleteShader(handle);
}
/////////////////////////////////////////////////////////////

const std::regex includeRegex("(?:^|\n)#include \"([^\"]+)\"");

inline std::string readShader(const std::filesystem::path& filepath, std::unordered_set<std::filesystem::path>& included) {
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

template <GLenum type>
void Shader<type>::load(const std::filesystem::path& filepath) {
    std::unordered_set<std::filesystem::path> included;
    std::string source = readShader(filepath, included);
#ifdef COMPOSE_SHADERS
    Common::writeToFile(source, Context::COMPOSED_SHADER_DIR / filepath.filepath);
#endif
    loadSource(source);
}

template <GLenum type>
void Shader<type>::loadSource(const std::string& source) {
    const char* sourcePtr = source.c_str();
    glShaderSource(handle, 1, &sourcePtr, nullptr);
    compile();
}

template <GLenum type>
void Shader<type>::compile() {
    glCompileShader(handle);
    int success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::array<char, 512> infoLog;
        glGetShaderInfoLog(handle, 512, nullptr, infoLog.data());
        throw std::runtime_error("Shader compilation failed: " + std::string(infoLog.data()));
    }
}