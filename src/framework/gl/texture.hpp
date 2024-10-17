#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <stdexcept>

#include <stb_image.h>
#include <stb_image_write.h>

#include <glad/glad.h>

#include "framework/common.hpp"
#include "framework/context.hpp"

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
 * @tparam target The target/type for which the texture should be created, e.g. `GL_TEXTURE_2D`, `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, etc.
 * In legacy OpenGL (< 4.5) the texture is bound to a target to interact with it.
 * In this versions a target is like a slot that can hold exactly one object.
 * Thankfully, since 4.5 this design choice was superseded by https://www.khronos.org/opengl/wiki/Direct_State_Access.
 */
template<GLenum target>
class Texture {

   public:

    /**
     * @brief Constructs a Texture object with the specified target.
     * The identifier of the OpenGL object is stored in `Texture::handle`.
     */
    Texture();

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
    Texture(Texture &&other) noexcept;

    /**
     * @brief Move assignment operator, deletes the current Texture object and invalidates the other Texture object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Texture object to move from.
     * @return A reference to the moved Texture object.
     */
    Texture &operator=(Texture &&other) noexcept;

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
     * @brief Allocates storage for a 2D texture.
     * @param internalFormat The internal format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param mipmaps The number of mipmaps to allocate.
     */
    void allocate2D(GLenum internalFormat, GLint width, GLint height, GLint mipmaps = 0);

    /**
     * @brief Loads a 2D texture from memory.
     * @note Prefer `Texture::load`, as this function is incomplete and requires the texture first to be bound.
     * @param texTarget The target texture type to load the texture into, cubemaps require special targets per face.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param data The image data.
     */
    void _load2D(GLenum texTarget, GLenum format, GLint width, GLint height, void* data);

    /**
     * @brief Loads a 2D texture from a file.
     * @note Prefer `Texture::load`, as this function is incomplete and requires the texture first to be bound.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param texTarget The target texture type to load the texture into, cubemaps require special targets per face.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param filepath The path to the image file.
     */
    void _load2D(GLenum texTarget, GLenum format, const std::filesystem::path& filepath);

    /**
     * @brief Loads a 3D texture slice from memory.
     * @note Prefer `Texture::load`, as this function is incomplete and requires the texture first to be bound.
     * @param zindex The z-index of the texture slice.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param data The image data.
     */
    void _load3D(GLint zindex, GLenum format, GLint width, GLint height, void* data);

    /**
     * @brief Loads a 3D texture slice from a file.
     * @note This function is incomplete and requires the texture first to be bound.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param zindex The z-index of the texture slice.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param filepath The path to the image file.
     */
    void _load3D(GLint zindex, GLenum format, const std::filesystem::path& filepath);

    /**
     * @brief Loads a texture from a file.
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param filepath The path to the image file.
     * @param mipmaps The number of mipmaps to generate (default is 0, which means to generate no mipmaps).
     */
    void load(GLenum format, const std::filesystem::path& filepath, GLint mipmaps = 0);

    /**
     * @brief Loads a cubemap texture from multiple image files.
     * Such cubemaps can be generated for example with this tool https://matheowis.github.io/HDRI-to-CubeMap/ (choose `.hdr` and last export option)
     * You can find free cubemaps at https://hdri-haven.com
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param filepaths An array of paths to the image files (6 files for each face of the cubemap in the order +X, -X, +Y, -Y, +Z, -Z).
     * @param mipmaps The number of mipmaps to generate (default is 0, which means to generate no mipmaps).
     * @note You may have to flip the cubemap by accessing it with `texture(tCubemap, rayDir * vec3(-1, 1, 1)).rgb;`
     */
    void loadCubemap(GLenum format, const std::array<std::filesystem::path, 6>& filepaths, GLint mipmaps = 0);

    /**
     * @brief Loads a cubemap texture from a directory containing 6 image files.
     * Such cubemaps can be generated for example with this tool https://matheowis.github.io/HDRI-to-CubeMap/ (choose .hdr and last export option)
     * You can find free cubemaps at https://hdri-haven.com
     * @throw `std::runtime_error` when the file could not be parsed.
     * @param format The format of the texture. E.g. for standard color use `GL_SRGB8_ALPHA8`, for HDR use `GL_RGBA32F` or `GL_RGBA16F` and for normal maps `GL_RGB8_SNORM`. See https://www.khronos.org/opengl/wiki/Image_Format for more information.
     * @param directory The path to the directory containing the image files as `px.hdr`, `nx.hdr`, `py.hdr`, `ny.hdr`, `pz.hdr`, and `nz.hdr`.
     * @param mipmaps The number of mipmaps to generate (default is 0, which means to generate no mipmaps).
     * @note You may have to flip the cubemap by accessing it with `texture(tCubemap, rayDir * vec3(-1, 1, 1)).rgb;`
     */
    void loadCubemap(GLenum format, const std::filesystem::path& directory, GLint mipmaps = 0);

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

   private:
    /**
     * @brief Releases the texture object.
     */
    void release();
};

/////////////////////// RAII behavior ///////////////////////
template<GLenum target>
Texture<target>::Texture() {
#ifdef MODERN_GL
    glCreateTextures(target, 1, &handle);
#else
    glGenTextures(1, &handle);
#endif
}

template<GLenum target>
Texture<target>::Texture(Texture<target> &&other) noexcept : handle(other.handle) {
    other.handle = 0;
}

template<GLenum target>
Texture<target> &Texture<target>::operator=(Texture<target> &&other) noexcept {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

template<GLenum target>
Texture<target>::~Texture() {
    release();
}

template<GLenum target>
void Texture<target>::release() {
    if (handle) glDeleteTextures(1, &handle);
}
/////////////////////////////////////////////////////////////

template<GLenum target>
void Texture<target>::bind() {
    glBindTexture(target, handle);
}

template<GLenum target>
void Texture<target>::bindTextureUnit(GLuint index) {
#ifdef MODERN_GL
    // On OpenGL 4.5+ one would use the DSA version glBindTextureUnit
    glBindTextureUnit(index, handle);
#else
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(target, handle);
#endif
}

/**
 * @brief Gets the number of channels from a base format, e.g. `GL_RED` returns 1, `GL_RGB` returns 3, etc.
 * @note See: https://gist.github.com/Kos/4739337
 */
inline GLsizei getChannels(GLenum baseFormat) {
    switch(baseFormat) {
        case GL_RED: return 1;
        case GL_RG: return 2;
        case GL_RGB: return 3;
        case GL_RGBA: return 4;
        default: throw std::runtime_error("Unsupported base format");
    }
}

/**
 * @brief Gets the base format from an internal format, e.g. `GL_R8` returns `GL_RED`, `GL_RGB8` returns `GL_RGB`, etc.
 * @note See: https://gist.github.com/Kos/4739337
 */
inline GLenum getBaseFormat(GLenum internalFormat) {
    switch (internalFormat) {
        case GL_R8:
        case GL_R16F:
        case GL_R32F:
        case GL_R8_SNORM:
            return GL_RED;
        case GL_RG8:
        case GL_RG16F:
        case GL_RG32F:
        case GL_RG8_SNORM:
            return GL_RG;
        case GL_RGB8:
        case GL_RGB16F:
        case GL_RGB32F:
        case GL_RGB8_SNORM:
        case GL_SRGB8:
            return GL_RGB;
        case GL_RGBA8:
        case GL_RGBA16F:
        case GL_RGBA32F:
        case GL_RGBA8_SNORM:
        case GL_SRGB8_ALPHA8:
            return GL_RGBA;
        case GL_DEPTH_COMPONENT32F:
            return GL_DEPTH_COMPONENT;
        case GL_DEPTH32F_STENCIL8:
            return GL_DEPTH_STENCIL;
        default: throw std::runtime_error("Unsupported internal format");
    }
    return GL_NONE;
}

/**
 * @brief Gets the preferred data type from an internal format, e.g. `GL_R8` returns `GL_UNSIGNED_BYTE`, `GL_RGB32F` returns `GL_FLOAT`, etc.
 * @note See: https://gist.github.com/Kos/4739337
 */
inline GLenum getDataType(GLenum internalFormat) {
    switch (internalFormat) {
        case GL_R8:
        case GL_RG8:
        case GL_RGB8:
        case GL_RGBA8:
        case GL_SRGB8:
        case GL_SRGB8_ALPHA8:
            return GL_UNSIGNED_BYTE;
        case GL_R8_SNORM:
        case GL_RG8_SNORM:
        case GL_RGB8_SNORM:
        case GL_RGBA8_SNORM:
            return GL_BYTE;
        case GL_R16F:
        case GL_RG16F:
        case GL_RGB16F:
        case GL_RGBA16F:
        case GL_R32F:
        case GL_RG32F:
        case GL_RGB32F:
        case GL_RGBA32F:
        case GL_DEPTH_COMPONENT32F:
            return GL_FLOAT;
        case GL_DEPTH32F_STENCIL8:
            return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
        default: throw std::runtime_error("Unsupported internal format");
    }
    return GL_NONE;

}

template<GLenum target>
void Texture<target>::allocate2D(GLenum internalFormat, GLint width, GLint height, GLint mipmaps) {
    #ifdef MODERN_GL
        glTextureStorage2D(handle, mipmaps + 1, internalFormat, width, height);
    #else
        GLenum baseFormat = getBaseFormat(internalFormat);
        GLenum dataType = getDataType(internalFormat);
        bind();
        glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmaps); // Must be set to avoid crashes on some drivers
        glTexImage2D(target, 0, internalFormat, width, height, 0, baseFormat, dataType, nullptr);
    #endif
}

template<GLenum target>
void Texture<target>::_load2D(GLenum texTarget, GLenum internalFormat, GLint width, GLint height, void* data) {
    GLenum baseFormat = getBaseFormat(internalFormat);
    GLenum dataType = getDataType(internalFormat);
    #ifdef MODERN_GL
        // This would be the immutable version:
        // glTexStorage2D(GL_TEXTURE_2D, levels, internalFormat, width, height);
        // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, baseFormat, dataType, data);
        // Note: On OpenGL 4.5+ one would use the DSA versions glTextureStorage2D and glTextureSubImage2D
        glTextureSubImage2D(handle, 0, 0, 0, width, height, baseFormat, dataType, data);
    #else
        glTexImage2D(texTarget, 0, internalFormat, width, height, 0, baseFormat, dataType, data);
    #endif
}

template<GLenum target>
void Texture<target>::_load2D(GLenum texTarget, GLenum internalFormat, const std::filesystem::path& filepath) {
    GLsizei width, height, channelsInFile;
    GLenum baseFormat = getBaseFormat(internalFormat);
    GLsizei channels = getChannels(baseFormat);
    GLenum dataType = getDataType(internalFormat);
    void* data;

    // Load image from file and read format
    Context::setWorkingDirectory(); // Ensure that the working directory is set correctly
    switch (dataType) {
        case GL_UNSIGNED_BYTE:
            data = stbi_load(filepath.string().c_str(), &width, &height, &channelsInFile, channels);
            break;
        case GL_BYTE:
            data = stbi_load(filepath.string().c_str(), &width, &height, &channelsInFile, channels);
            // Convert from unsigned to signed bytes
            for (int i = 0; i < width * height * channels; i++) static_cast<char*>(data)[i] -= 128;
            break;
        case GL_FLOAT:
            data = stbi_loadf(filepath.string().c_str(), &width, &height, &channelsInFile, channels);
            break;
        default: throw std::runtime_error("Unsupported texture format");
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.string() + ": " + stbi_failure_reason());

    _load2D(texTarget, internalFormat, width, height, data);

    // Free image data
    stbi_image_free(data);
}

template<GLenum target>
void Texture<target>::_load3D(GLint zindex, GLenum internalFormat, GLint width, GLint height, void* data) {
    GLenum baseFormat = getBaseFormat(internalFormat);
    GLenum dataType = getDataType(internalFormat);
    #ifdef MODERN_GL
        glTextureSubImage3D(handle, 0, 0, 0, zindex, width, height, 1, baseFormat, dataType, data);
    #else
        glTexSubImage3D(target, 0, 0, 0, zindex, width, height, 1, baseFormat, dataType, data);
    #endif
}

template<GLenum target>
void Texture<target>::_load3D(GLint zindex, GLenum internalFormat, const std::filesystem::path& filepath) {
    GLsizei width, height, channelsInFile;
    GLenum baseFormat = getBaseFormat(internalFormat);
    GLsizei channels = getChannels(baseFormat);
    GLenum dataType = getDataType(internalFormat);
    void* data;

    // Load image from file and read format
    Context::setWorkingDirectory(); // Ensure that the working directory is set correctly
    switch (dataType) {
        case GL_UNSIGNED_BYTE:
            data = stbi_load(filepath.string().c_str(), &width, &height, &channelsInFile, channels);
            break;
        case GL_BYTE:
            data = stbi_load(filepath.string().c_str(), &width, &height, &channelsInFile, channels);
            // Convert from unsigned to signed bytes
            for (int i = 0; i < width * height * channels; i++) static_cast<char*>(data)[i] -= 128;
            break;
        case GL_FLOAT:
            data = stbi_loadf(filepath.string().c_str(), &width, &height, &channelsInFile, channels);
            break;
        default: throw std::runtime_error("Unsupported texture format");
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.string() + ": " + stbi_failure_reason());

    _load3D(zindex, internalFormat, width, height, data);

    // Free image data
    stbi_image_free(data);
}

template<GLenum target>
void Texture<target>::load(GLenum internalFormat, const std::filesystem::path& filepath, GLint mipmaps) {
    stbi_set_flip_vertically_on_load(true); // OpenGL expects the origin to be at the bottom left

#ifdef MODERN_GL
    GLsizei width, height, channels;
    Context::setWorkingDirectory(); // Ensure that the working directory is set correctly
    if (!stbi_info(filepath.string().c_str(), &width, &height, &channels))
        throw std::runtime_error("Failed to parse image " + filepath.string() + ": " + stbi_failure_reason());
    glTextureStorage2D(handle, mipmaps + 1, internalFormat, width, height);
#else
    bind();
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmaps); // Must be set to avoid crashes on some drivers
#endif

    _load2D(target, internalFormat, filepath);

    // Generate mipmaps
#ifdef MODERN_GL
    if (mipmaps) glGenerateTextureMipmap(handle);
#else
    if (mipmaps) glGenerateMipmap(target);
#endif
}

template<GLenum target>
void Texture<target>::loadCubemap(GLenum internalFormat, const std::array<std::filesystem::path, 6>& filepaths, GLint mipmaps) {
    // For seamless cubemaps, call glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS)
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    stbi_set_flip_vertically_on_load(false); // Do not flip cubemap faces

#ifdef MODERN_GL
    // Should always be set for cubemaps, see https://www.khronos.org/opengl/wiki_opengl/index.php?title=Common_Mistakes&section=14#Creating_a_Cubemap_Texture
    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    GLsizei width, height, channels;
    Context::setWorkingDirectory(); // Ensure that the working directory is set correctly
    if (!stbi_info(filepaths[0].string().c_str(), &width, &height, &channels))
        throw std::runtime_error("Failed to parse image " + filepaths[0].string() + ": " + stbi_failure_reason());
    glTextureStorage2D(handle, mipmaps + 1, internalFormat, width, height);
    for (int i = 0; i < filepaths.size(); i++) {
        _load3D(i, internalFormat, filepaths[i]);
    }
#else
    bind();
    // Should always be set for cubemaps, see https://www.khronos.org/opengl/wiki_opengl/index.php?title=Common_Mistakes&section=14#Creating_a_Cubemap_Texture
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmaps); // Must be set to avoid crashes on some drivers
    _load2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, internalFormat, filepaths[0]);
    _load2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, internalFormat, filepaths[1]);
    _load2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, internalFormat, filepaths[2]);
    _load2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, internalFormat, filepaths[3]);
    _load2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, internalFormat, filepaths[4]);
    _load2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, internalFormat, filepaths[5]);
#endif

    // Generate mipmaps
#ifdef MODERN_GL
    if (mipmaps) glGenerateTextureMipmap(handle);
#else
    if (mipmaps) glGenerateMipmap(target);
#endif
}

template<GLenum target>
void Texture<target>::loadCubemap(GLenum internalFormat, const std::filesystem::path& directory, GLint mipmaps) {
    std::array<std::filesystem::path, 6> filepaths = {
        directory / "px.hdr",
        directory / "nx.hdr",
        directory / "py.hdr",
        directory / "ny.hdr",
        directory / "pz.hdr",
        directory / "nz.hdr"
    };
    loadCubemap(internalFormat, filepaths, mipmaps);
}

template<GLenum target>
bool Texture<target>::writeToFile(const std::filesystem::path& filepath) {
    GLint width, height;
    GLint internalFormat;

#ifdef MODERN_GL
    glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
#else
    bind();
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(target, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
#endif

    GLenum baseFormat = getBaseFormat(internalFormat);
    GLenum dataType = getDataType(internalFormat);
    int channels = 4; // glTexImage2D always returns 4 channels

    Context::setWorkingDirectory(); // Ensure that the working directory is set correctly
    stbi_flip_vertically_on_write(true);

    if (dataType == GL_FLOAT) {
        auto floatData = std::make_unique<float[]>(width * height * channels);

    #ifdef MODERN_GL
        glGetTextureImage(handle, 0, baseFormat, dataType, width * height * channels * sizeof(float), floatData.get());
    #else
        glGetTexImage(target, 0, baseFormat, dataType, floatData.get());
    #endif

        return stbi_write_hdr(filepath.string().c_str(), width, height, channels, floatData.get());
    } else if (dataType == GL_UNSIGNED_BYTE || dataType == GL_BYTE) {
        auto byteData = std::make_unique<unsigned char[]>(width * height * channels);

    #ifdef MODERN_GL
        glGetTextureImage(handle, 0, baseFormat, dataType, width * height * channels * sizeof(unsigned char), byteData.get());
    #else
        glGetTexImage(target, 0, baseFormat, dataType, byteData.get());
    #endif

        // Convert from signed to unsigned bytes
        if (dataType == GL_BYTE)
            for (int i = 0; i < width * height * channels; i++)
                byteData[i] += 128;
        
        auto ext = filepath.extension();
        if (ext == ".bmp")
            return stbi_write_bmp(filepath.string().c_str(), width, height, channels, byteData.get());
        else if (ext == ".tga")
            return stbi_write_tga(filepath.string().c_str(), width, height, channels, byteData.get());
        else if (ext == ".jpg" || ext == ".jpeg")
            return stbi_write_jpg(filepath.string().c_str(), width, height, channels, byteData.get(), 95);
        else if (ext == ".png")
            return stbi_write_png(filepath.string().c_str(), width, height, channels, byteData.get(), width * channels);
        else
            throw std::runtime_error("Unsupported image format");
    } else throw std::runtime_error("Unsupported texture format");
}

template<GLenum target>
void Texture<target>::set(GLenum parameter, GLenum value) {
#ifdef MODERN_GL
    glTextureParameteri(handle, parameter, value);
#else
    bind();
    glTexParameteri(target, parameter, value);
#endif
}

template<GLenum target>
void Texture<target>::set(GLenum parameter, GLint value) {
#ifdef MODERN_GL
    glTextureParameteri(handle, parameter, value);
#else
    bind();
    glTexParameteri(target, parameter, value);
#endif
}

template<GLenum target>
GLint Texture<target>::get(GLenum parameter, GLint level) {
    GLint value;
#ifdef MODERN_GL
    glGetTextureLevelParameteriv(handle, level, parameter, &value);
#else
    bind();
    glGetTexLevelParameteriv(target, level, parameter, &value);
#endif
    return value;
}