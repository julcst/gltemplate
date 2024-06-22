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
Texture::Texture() {
    glGenTextures(1, &handle);
    assert(handle);
}

Texture::Texture(Texture&& other) : handle(other.handle) {
    other.handle = 0;
}

Texture& Texture::operator=(Texture&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
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

void Texture::bind(GLenum type) {
    glBindTexture(type, handle);
}

void Texture::bind(GLenum type, GLuint index) {
    // On OpenGL 4.5+ one would use the DSA version glBindTextureUnit
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(type, handle);
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

GLenum getBaseFormat(GLenum internalformat) {
    switch (internalformat) {
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
        case GL_DEPTH32F_STENCIL8:
            return GL_DEPTH_STENCIL;
        default: assert(false);
    }
    return GL_NONE;
}

void Texture::_load(GLenum target, Format format, const std::filesystem::path& filepath) {
    int width, height, channels;
    GLenum type;
    void* data;

    // Load image from file and read format
    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_set_flip_vertically_on_load(true);
    switch (format) {
        case Format::LINEAR8:
        case Format::SRGB8:
        case Format::NORMAL8:
            type = GL_UNSIGNED_BYTE;
            data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
            break;
        case Format::FLOAT16:
        case Format::FLOAT32:
            type = GL_FLOAT;
            data = stbi_loadf(filepath.c_str(), &width, &height, &channels, 0);
            break;
        default: assert(false);
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.native() + ": " + stbi_failure_reason());

    GLenum internalformat = getInternalFormat(format, channels);
    GLenum baseformat = getBaseFormat(internalformat);

    // Upload texture data
    // This would be the immutable version:
    // Note: On OpenGL 4.5+ one would use the DSA versions glTextureStorage2D and glTextureSubImage2D
    // glTexStorage2D(GL_TEXTURE_2D, mipmaps, internalformat, width, height);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, baseformat, type, data);
    glTexImage2D(target, 0, internalformat, width, height, 0, baseformat, type, data);

    // Free image data
    stbi_image_free(data);
}

bool Texture::writeToFile(const std::filesystem::path& filepath) {
    bind(GL_TEXTURE_2D);

    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    GLenum internalformat;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
    GLenum baseformat = getBaseFormat(internalformat);
    int channels;
    switch (baseformat) {
        case GL_RED: channels = 1; break;
        case GL_RG: channels = 2; break;
        case GL_RGB: channels = 3; break;
        case GL_RGBA: channels = 4; break;
        default: assert(false);
    }

    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_flip_vertically_on_write(true);
    GLenum type;
    if (internalformat == GL_RGBA32F || internalformat == GL_RGB32F || internalformat == GL_RG32F || internalformat == GL_R32F) {
        type = GL_FLOAT;
        float fData[width * height * 4];
        glGetTexImage(GL_TEXTURE_2D, 0, baseformat, type, fData);

        return stbi_write_hdr(filepath.c_str(), width, height, channels, fData);
    } else {
        type = GL_UNSIGNED_BYTE;
        u_char data[width * height * 4];
        glGetTexImage(GL_TEXTURE_2D, 0, baseformat, type, data);
        
        auto ext = filepath.extension();
        if (ext == ".bmp") return stbi_write_bmp(filepath.c_str(), width, height, channels, data);
        else if (ext == ".tga") return stbi_write_tga(filepath.c_str(), width, height, channels, data);
        else if (ext == ".jpg" || ext == ".jpeg") return stbi_write_jpg(filepath.c_str(), width, height, channels, data, 95);
        else if (ext == ".png") return stbi_write_png(filepath.c_str(), width, height, channels, data, width * channels);
        else return false;
    }
}

void Texture::load(Format format, const std::filesystem::path& filepath, GLsizei mipmaps) {
    bind(GL_TEXTURE_2D);
    
    _load(GL_TEXTURE_2D, format, filepath);

    // Generate mipmaps
    if (mipmaps > 0) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::loadCubemap(Format format, const std::array<std::filesystem::path, 6>& filepaths, GLsizei mipmaps) {
    // For seamless cubemaps, call glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS)
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    bind(GL_TEXTURE_CUBE_MAP);

    _load(GL_TEXTURE_CUBE_MAP_POSITIVE_X, format, filepaths[0]);
    _load(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, format, filepaths[1]);
    _load(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, format, filepaths[2]);
    _load(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, format, filepaths[3]);
    _load(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, format, filepaths[4]);
    _load(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, format, filepaths[5]);

    if (mipmaps > 0) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}