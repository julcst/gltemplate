#include "texture.hpp"

#include <glbinding/gl/gl.h>
using namespace gl;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#ifndef __STDC_LIB_EXT1__
#define __STDC_LIB_EXT1__
#define sprintf_s snprintf
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <array>
#include <cassert>
#include <string>
#include <stdexcept>

#include "framework/common.hpp"
#include "framework/context.hpp"

/////////////////////// RAII behavior ///////////////////////
Texture::Texture(GLenum type) : type(type) {
#ifdef MODERN_GL
    glCreateTextures(type, 1, &handle);
#else
    glGenTextures(1, &handle);
#endif
    assert(handle);
}

Texture::Texture(Texture&& other) : handle(other.handle), type(other.type) {
    other.handle = 0;
}

Texture& Texture::operator=(Texture&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        type = other.type;
        other.handle = 0;
    }
    return *this;
}

Texture::~Texture() {
    release();
}

void Texture::release() {
    if (handle) glDeleteTextures(1, &handle);
}
/////////////////////////////////////////////////////////////

void Texture::bind() {
    glBindTexture(type, handle);
}

void Texture::bindTextureUnit(GLuint index) {
#ifdef MODERN_GL
    // On OpenGL 4.5+ one would use the DSA version glBindTextureUnit
    glBindTextureUnit(index, handle);
#else
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(type, handle);
#endif
}

GLenum getInternalFormat(Texture::Format format, int channels) {
    switch (format) {
        case Texture::Format::LINEAR8:
            switch (channels) {
                case 1: return GL_R8;
                case 2: return GL_RG8;
                case 3: return GL_RGB8;
                case 4: return GL_RGBA8;
                default: assert(false);
            }
        case Texture::Format::SRGB8:
            switch (channels) {
                case 1: assert(false);
                case 2: assert(false);
                case 3: return GL_SRGB8;
                case 4: return GL_SRGB8_ALPHA8;
                default: assert(false);
            }
        case Texture::Format::FLOAT16:
            switch (channels) {
                case 1: return GL_R16F;
                case 2: return GL_RG16F;
                case 3: return GL_RGB16F;
                case 4: return GL_RGBA16F;
                default: assert(false);
            }
        case Texture::Format::FLOAT32:
            switch (channels) {
                case 1: return GL_R32F;
                case 2: return GL_RG32F;
                case 3: return GL_RGB32F;
                case 4: return GL_RGBA32F;
                default: assert(false);
            }
        case Texture::Format::NORMAL8:
            switch (channels) {
                case 1: return GL_R8_SNORM;
                case 2: return GL_RG8_SNORM;
                case 3: return GL_RGB8_SNORM;
                case 4: return GL_RGBA8_SNORM;
                default: assert(false);
            }
        default: assert(false);
    }
    return GL_NONE;
}

GLenum getBaseFormat(GLenum internalFormat) {
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
        default: assert(false);
    }
    return GL_NONE;
}

/** See: https://gist.github.com/Kos/4739337 */
GLenum getType(GLenum internalFormat) {
    switch (internalFormat) {
        case GL_R8:
        case GL_RG8:
        case GL_RGB8:
        case GL_RGBA8:
        case GL_R8_SNORM:
        case GL_RG8_SNORM:
        case GL_RGB8_SNORM:
        case GL_RGBA8_SNORM:
        case GL_SRGB8:
        case GL_SRGB8_ALPHA8:
            return GL_UNSIGNED_BYTE;
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
        default: assert(false);
    }
    return GL_NONE;

}

void Texture::_load2D(GLenum target, Format format, const std::filesystem::path& filepath) {
    GLsizei width, height, channels;
    GLenum dataType;
    void* data;

    // Load image from file and read format
    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_set_flip_vertically_on_load(true);
    switch (format) {
        case Format::LINEAR8:
        case Format::SRGB8:
        case Format::NORMAL8:
            dataType = GL_UNSIGNED_BYTE;
            data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
            break;
        case Format::FLOAT16:
        case Format::FLOAT32:
            dataType = GL_FLOAT;
            data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);
            break;
        default: assert(false);
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.native() + ": " + stbi_failure_reason());

    GLenum internalFormat = getInternalFormat(format, channels);
    GLenum baseFormat = getBaseFormat(internalFormat);

    // Upload texture data
#ifdef MODERN_GL
    // This would be the immutable version:
    // glTexStorage2D(GL_TEXTURE_2D, levels, internalFormat, width, height);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, baseFormat, dataType, data);
    // Note: On OpenGL 4.5+ one would use the DSA versions glTextureStorage2D and glTextureSubImage2D
    glTextureStorage2D(handle, 1, internalFormat, width, height);
    glTextureSubImage2D(handle, 0, 0, 0, width, height, baseFormat, dataType, data);
#else
    bind();
    glTexImage2D(target, 0, internalFormat, width, height, 0, baseFormat, dataType, data);
#endif
    // Free image data
    stbi_image_free(data);
}

void Texture::_load3D(GLint zindex, Format format, const std::filesystem::path& filepath) {
    GLsizei width, height, channels;
    GLenum dataType;
    void* data;

    // Load image from file and read format
    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_set_flip_vertically_on_load(true);
    switch (format) {
        case Format::LINEAR8:
        case Format::SRGB8:
        case Format::NORMAL8:
            dataType = GL_UNSIGNED_BYTE;
            data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
            break;
        case Format::FLOAT16:
        case Format::FLOAT32:
            dataType = GL_FLOAT;
            data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);
            break;
        default: assert(false);
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.native() + ": " + stbi_failure_reason());

    GLenum internalFormat = getInternalFormat(format, channels);
    GLenum baseFormat = getBaseFormat(internalFormat);

    // Upload texture data
#ifdef MODERN_GL
    glTextureSubImage3D(handle, 0, 0, 0, zindex, width, height, 1, baseFormat, dataType, data);
#else
    bind();
    glTexSubImage3D(type, 0, 0, 0, zindex, width, height, 1, baseFormat, dataType, data);
#endif
}

void Texture::load(Format format, const std::filesystem::path& filepath, bool mipmaps) {
    _load2D(type, format, filepath);

    // Generate mipmaps
#ifdef MODERN_GL
    if (mipmaps) glGenerateTextureMipmap(handle);
#else
    if (mipmaps) glGenerateMipmap(type);
#endif
}

void Texture::loadCubemap(Format format, const std::array<std::filesystem::path, 6>& filepaths, bool mipmaps) {
    // For seamless cubemaps, call glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS)
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

#ifdef MODERN_GL
    GLsizei width, height, channels;
    if (!stbi_info(filepaths[0].c_str(), &width, &height, &channels)) {
        throw std::runtime_error("Failed to parse image " + filepaths[0].native() + ": " + stbi_failure_reason());
    }
    GLenum internalFormat = getInternalFormat(format, channels);
    glTextureStorage2D(handle, 1, internalFormat, width, height);
    for (int i = 0; i < 6; i++) {
        _load3D(i, format, filepaths[i]);
    }
#else
    _load2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, format, filepaths[0]);
    _load2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, format, filepaths[1]);
    _load2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, format, filepaths[2]);
    _load2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, format, filepaths[3]);
    _load2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, format, filepaths[4]);
    _load2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, format, filepaths[5]);
#endif

    // Generate mipmaps
#ifdef MODERN_GL
    if (mipmaps) glGenerateTextureMipmap(handle);
#else
    if (mipmaps) glGenerateMipmap(type);
#endif
}

void Texture::loadCubemap(Format format, const std::filesystem::path& directory, bool mipmaps) {
    std::array<std::filesystem::path, 6> filepaths = {
        directory / "px.hdr",
        directory / "nx.hdr",
        directory / "py.hdr",
        directory / "ny.hdr",
        directory / "pz.hdr",
        directory / "nz.hdr"
    };
    loadCubemap(format, filepaths, mipmaps);
}

bool Texture::writeToFile(const std::filesystem::path& filepath) {
    int width, height;
    GLenum internalFormat;

#ifdef MODERN_GL
    glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTextureLevelParameteriv(handle, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
#else
    bind();
    glGetTexLevelParameteriv(type, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(type, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(type, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
#endif

    GLenum baseFormat = getBaseFormat(internalFormat);
    GLenum dataType = getType(internalFormat);
    int channels = 4; // glTexImage2D always returns 4 channels

    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_flip_vertically_on_write(true);

    if (dataType == GL_FLOAT) {
        float floatData[width * height * channels];

    #ifdef MODERN_GL
        glGetTextureImage(handle, 0, baseFormat, dataType, sizeof(floatData), floatData);
    #else
        glGetTexImage(GL_TEXTURE_2D, 0, baseFormat, dataType, floatData);
    #endif

        return stbi_write_hdr(filepath.c_str(), width, height, channels, floatData);
    } else if (dataType == GL_UNSIGNED_BYTE) {
        u_char byteData[width * height * channels];

    #ifdef MODERN_GL
        glGetTextureImage(handle, 0, baseFormat, dataType, sizeof(byteData), byteData);
    #else
        glGetTexImage(GL_TEXTURE_2D, 0, baseFormat, dataType, byteData);
    #endif
        
        auto ext = filepath.extension();
        if (ext == ".bmp") return stbi_write_bmp(filepath.c_str(), width, height, channels, byteData);
        else if (ext == ".tga") return stbi_write_tga(filepath.c_str(), width, height, channels, byteData);
        else if (ext == ".jpg" || ext == ".jpeg") return stbi_write_jpg(filepath.c_str(), width, height, channels, byteData, 95);
        else if (ext == ".png") return stbi_write_png(filepath.c_str(), width, height, channels, byteData, width * channels);
        else return false;
    } else return false;
}

void Texture::set(GLenum parameter, GLint value) {
#ifdef MODERN_GL
    glTextureParameteri(handle, parameter, value);
#else
    bind();
    glTexParameteri(type, parameter, value);
#endif
}

int Texture::get(GLenum parameter, GLint level) {
    GLint value;
#ifdef MODERN_GL
    glGetTextureLevelParameteriv(handle, level, parameter, &value);
#else
    bind();
    glGetTexLevelParameteriv(type, level, parameter, &value);
#endif
    return value;
}