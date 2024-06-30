#pragma once

#include <glbinding/gl46core/gl.h>

#include <string>
#include <filesystem>

#include "buffer.hpp"
#include "shader.hpp"

using namespace gl46core;

/**
 * @file program.hpp
 * @brief Defines a Program class wrapper around the OpenGL program object.
 */

/**
 * @class Program
 * @brief RAII wrapper for OpenGL program with helper functions for loading and linking shaders and uniform setting.
 * See https://www.khronos.org/opengl/wiki/Program_Object for more information.
 */
class Program {
   public:

    /**
     * @brief Constructs a Program object.
     * The identifier of the OpenGL object is stored in `Program::handle`.
     */
    Program();

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Program(const Program&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Program& operator=(const Program&) = delete;
    
    /**
     * @brief Move constructor, invalidates the other Program object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Program object to move from.
     */
    Program(Program&& other);

    /**
     * @brief Move assignment operator, deletes the current Program object and invalidates the other Program object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Program object to move from.
     * @return A reference to the moved Program object.
     */
    Program& operator=(Program&& other);

    /**
     * @brief Destructor, frees the OpenGL object.
     */
    ~Program();

    /**
     * @brief Loads and links the vertex and fragment shaders from the specified file paths.
     * @throw `std::runtime_error` when the shaders could not be loaded or linked. 
     * @param vs The file path to the vertex shader source code.
     * @param fs The file path to the fragment shader source code.
     * @note Equivalent to calling
     * `attach(vs, GL_VERTEX_SHADER);`
     * `attach(fs, GL_FRAGMENT_SHADER);`
     * `link();`
     */
    void load(const std::filesystem::path& vs, const std::filesystem::path& fs);

    /**
     * @brief Attaches a shader to the program.
     * Internally, this function creates a Shader object, calls Shader::load, attaches it to the program, and deletes the Shader object as it is no longer needed.
     * @throw `std::runtime_error` when the shader could not be loaded.
     * @param filepath The file path to the shader source code.
     * @param type The type of shader, e.g. `GL_VERTEX_SHADER`, `GL_FRAGMENT_SHADER`, `GL_COMPUTE_SHADER`.
     */
    void attach(const std::filesystem::path& filepath, GLenum type);

    /**
     * @brief Attaches a shader to the program.
     * @param shader The Shader object to attach.
     */
    void attach(const Shader& shader);

    /**
     * @brief Attaches a shader to the program.
     * @param shader The handle of the shader object to attach.
     */
    void attach(GLuint shader);

    /**
     * @brief Links the program.
     * @throw `std::runtime_error` when the program could not be linked.
     */
    void link();

    /**
     * @brief Uses the program for rendering.
     */
    void use();

    /**
     * @brief Retrieves the location of a uniform variable.
     * @param name The name of the uniform variable.
     * @return The location of the uniform variable.
     */
    GLuint uniform(const std::string& name);

    /**
     * @brief Binds a uniform block to the specified index.
     * @param loc The name of the uniform block.
     * @param index The index to bind the uniform block to.
     * @note The uniform buffer object must be bound to the same index with `Buffer::bind(index)`.
     */
    void bindUBO(const std::string& loc, GLuint index);

    /**
     * @brief Binds a texture sampler to the specified texture unit.
     * @param loc The name of the texture sampler.
     * @param index The index of the texture unit to bind the texture sampler to.
     * @note The texture must be bound to the same index with `Texture::bindTextureUnit(index)`.
     */
    void bindTextureUnit(const std::string& loc, GLint index);

    /**
     * @brief Sets the value of a uniform variable.
     * @param loc The location of the uniform variable.
     * @param value The value to set the uniform variable to.
     */
    void set(GLuint loc, const GLint value);
    void set(GLuint loc, const std::vector<GLint>& values);
    void set(GLuint loc, const GLuint value);
    void set(GLuint loc, const std::vector<GLuint>& values);
    void set(GLuint loc, const GLfloat value);
    void set(GLuint loc, const std::vector<GLfloat>& values);
    void set(GLuint loc, const glm::ivec2& value);
    void set(GLuint loc, const std::vector<glm::ivec2>& values);
    void set(GLuint loc, const glm::vec2& value);
    void set(GLuint loc, const std::vector<glm::vec2>& values);
    void set(GLuint loc, const glm::ivec3& value);
    void set(GLuint loc, const std::vector<glm::ivec3>& values);
    void set(GLuint loc, const glm::vec3& value);
    void set(GLuint loc, const std::vector<glm::vec3>& values);
    void set(GLuint loc, const glm::ivec4& value);
    void set(GLuint loc, const std::vector<glm::ivec4>& values);
    void set(GLuint loc, const glm::vec4& value);
    void set(GLuint loc, const std::vector<glm::vec4>& values);
    void set(GLuint loc, const glm::mat2& value);
    void set(GLuint loc, const std::vector<glm::mat2>& values);
    void set(GLuint loc, const glm::mat3& value);
    void set(GLuint loc, const std::vector<glm::mat3>& values);
    void set(GLuint loc, const glm::mat4& value);
    void set(GLuint loc, const std::vector<glm::mat4>& values);

    /**
     * @brief Sets the value of a uniform variable.
     * @param loc The name of the uniform variable as a string.
     * @param value The value to set the uniform variable to.
     * @tparam T Matches the value types implemented in Program::set(GLuint, ...)`
     */
    template <typename T>
    void set(const std::string& loc, const T& value);

    /**
     * @brief The unique handle that identifies the program object on the GPU.
     */
    GLuint handle;

   private:
    
    /**
     * @brief Frees the OpenGL object.
     */
    void release();
};

template <typename T>
inline void Program::set(const std::string& loc, const T& value) {
    Program::set(Program::uniform(loc), value);
}