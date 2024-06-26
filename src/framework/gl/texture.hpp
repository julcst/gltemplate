#pragma once

#include <array>
#include <filesystem>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

using namespace gl;

/**
 * @file texture.hpp
 * @brief Defines a Texture class wrapper around the OpenGL texture object.
 */

/**
 * @class Texture
 * @brief RAII wrapper for OpenGL texture with helper functions for loading 2D textures and cubemaps.
 * For OpenGL 4.1 the textures are mutable, mutable texture creation with `glTexImage2D` though should be avoided because it is slower and more error-prone.
 * Thus with OpenGL 4.6 (`#define MODERN_GL`) this code uses immutable textures with `glTextureStorage2D` and DSA functions (https://www.khronos.org/opengl/wiki/Direct_State_Access).
 * See https://www.khronos.org/opengl/wiki/Texture for more information.
 */
class Texture {

   public:
   
    /**
     * @enum Format
     * @brief Formats that can be used to specify how to load a texture.
     * For standard color textures, use `SRGB8`.
     * For normal maps, use `NORMAL8`.
     * For HDR textures, use `FLOAT16` or `FLOAT32`.
     * See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     */
    enum class Format {
        LINEAR8,            // 8-bit unsigned normalized integer
        NORMAL8,            // 8-bit signed normalized integer
        SRGB8,              // 8-bit sRGB with linear alpha
        FLOAT16,            // 16-bit floating point
        FLOAT32,            // 32-bit floating point
    };

    /**
     * @brief Constructs a Texture object with the specified target.
     * The identifier of the OpenGL object is stored in `Texture::handle`.
     * Initialize a Texture object like this in the header file: `Texture texture { GL_TEXTURE_2D };`
     * @param target The type of texture, e.g. `GL_TEXTURE_2D`, `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, etc.
     */
    Texture(GLenum target);

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Texture(const Texture&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Texture& operator=(const Texture&) = delete;

    /**
     * @brief Move constructor, invalidates the other Texture object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Texture object to move from.
     */
    Texture(Texture&& other);

    /**
     * @brief Move assignment operator, deletes the current Texture object and invalidates the other Texture object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Texture object to move from.
     * @return A reference to the moved Texture object.
     */
    Texture& operator=(Texture&& other);

    /**
     * @brief Destructor, frees the OpenGL object.
     */
    ~Texture();

    /**
     * @brief Binds the texture object to the target specified in `Texture::target`
     */
    void bind();

    /**
     * @brief Binds the texture object to a specific texture unit.
     * @note The texture unit must also be bound with `Program::bindTextureUnit` to read or write from/to the texture
     * @param index The index of the texture unit to bind to.
     */
    void bindTextureUnit(GLuint index);

    /**
     * @brief Loads a 2D texture from a file.
     * @note Prefer `Texture::load`, as this function is incomplete and requires the texture first to be bound.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param target The target texture type to load the texture into, default is `Texture::target` but cubemaps require special targets per face.
     * @param format The format of the texture.
     * @param filepath The path to the image file.
     */
    void _load2D(GLenum target, Format format, const std::filesystem::path& filepath);

    /**
     * @brief Loads a 3D texture slice from a file.
     * @note This function is incomplete and requires the texture first to be bound.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param zindex The z-index of the texture slice.
     * @param format The format of the texture.
     * @param filepath The path to the image file.
     */
    void _load3D(GLint zindex, Format format, const std::filesystem::path& filepath);

    /**
     * @brief Loads a texture from a file.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param format The format of the texture.
     * @param filepath The path to the image file.
     * @param mipmaps The number of mipmaps to generate (default is 0, which means to generate no mipmaps).
     */
    void load(Format format, const std::filesystem::path& filepath, GLint mipmaps = 0);

    /**
     * @brief Loads a cubemap texture from multiple image files.
     * Such cubemaps can be generated for example with this tool https://matheowis.github.io/HDRI-to-CubeMap/ (choose `.hdr` and last export option)
     * You can find free cubemaps at https://hdri-haven.com
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param format The format of the texture.
     * @param filepaths An array of paths to the image files (6 files for each face of the cubemap in the order +X, -X, +Y, -Y, +Z, -Z).
     * @param mipmaps The number of mipmaps to generate (default is 0, which means to generate no mipmaps).
     * @note You may have to flip the cubemap by accessing it with `texture(tCubemap, rayDir * vec3(-1, 1, 1)).rgb;`
     */
    void loadCubemap(Format format, const std::array<std::filesystem::path, 6>& filepaths, GLint mipmaps = 0);

    /**
     * @brief Loads a cubemap texture from a directory containing 6 image files.
     * Such cubemaps can be generated for example with this tool https://matheowis.github.io/HDRI-to-CubeMap/ (choose .hdr and last export option)
     * You can find free cubemaps at https://hdri-haven.com
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param format The format of the texture.
     * @param directory The path to the directory containing the image files as `px.hdr`, `nx.hdr`, `py.hdr`, `ny.hdr`, `pz.hdr`, and `nz.hdr`.
     * @param mipmaps The number of mipmaps to generate (default is 0, which means to generate no mipmaps).
     * @note You may have to flip the cubemap by accessing it with `texture(tCubemap, rayDir * vec3(-1, 1, 1)).rgb;`
     */
    void loadCubemap(Format format, const std::filesystem::path& directory, GLint mipmaps = 0);

    /**
     * @brief Writes the texture data to a file.
     * @param filepath The path to the output file.
     * @return True if the write operation was successful, false otherwise.
     */
    bool writeToFile(const std::filesystem::path& filepath);

    /**
     * @brief Sets a texture parameter with a `GLenum` value.
     * For example use s`et(GL_TEXTURE_MAG_FILTER, GL_LINEAR)` and `set(GL_TEXTURE_MIN_FILTER, GL_LINEAR)` to enable bilinear filtering.
     * @param parameter The parameter to set.
     * @param value The value to set.
     */
    void set(GLenum parameter, GLenum value);

    /**
     * @brief Sets a texture parameter with a `GLint` value.
     * @param parameter The parameter to set.
     * @param value The value to set.
     */
    void set(GLenum parameter, GLint value);

    /**
     * @brief Gets a texture parameter with a `GLint` value.
     * For example use `get(GL_TEXTURE_WIDTH, 0)` to get the width of the image.
     * @param parameter The parameter to get.
     * @param level The mipmap level to get the parameter from, 0 is the base level.
     * @return The value of the parameter.
     */
    GLint get(GLenum parameter, GLint level);

    /**
     * @brief The unique handle that identifies the texture object on the GPU.
     */
    GLuint handle;  

    /**
     * @brief The type of the texture, e.g. `GL_TEXTURE_2D`, `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, etc.
     * In legacy OpenGL (< 4.5) the texture is bound to a target to interact with it.
     * In this versions a target is like a slot that can hold exactly one object.
     * Thankfully, since 4.5 this design choice was superseded by https://www.khronos.org/opengl/wiki/Direct_State_Access.
     */
    GLenum target;

   private:
    /**
     * @brief Releases the texture object.
     */
    void release();
};