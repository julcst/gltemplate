#pragma once

#include <glbinding/gl46core/gl.h>
using namespace gl46core;

#include <glm/glm.hpp>

#include <filesystem>

/**
 * @file shader.hpp
 * @brief Defines a Shader class wrapper around the OpenGL shader object.
 */

/**
 * @class Shader
 * @brief RAII wrapper for OpenGL shader with helper functions for loading and compiling shaders.
 * See https://www.khronos.org/opengl/wiki/Shader_Compilation for more information.
 */
class Shader {
   public:
    /**
     * @brief Constructs a Shader object with the specified target.
     * The identifier of the OpenGL object is stored in `Shader::handle`.
     * Initialize a Shader object like this in the header file: `Shader shader { GL_VERTEX_SHADER };`
     * @param target The type of shader, e.g. `GL_VERTEX_SHADER`, `GL_FRAGMENT_SHADER`, `GL_COMPUTE_SHADER`.
     */
    Shader(GLenum type);

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