#include "texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <cassert>
#include <string>
#include <stdexcept>

#include "common.hpp"

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

void Texture::bind(Type type) {
    glBindTexture(static_cast<GLenum>(type), handle);
}

void Texture::bind(Type type, GLuint index) {
    // On OpenGL 4.5+ one would use the DSA version glBindTextureUnit
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(static_cast<GLenum>(type), handle);
}

void Texture::load(Format format, const std::string& filename, GLsizei mipmaps) {
    int width, height, channels;
    GLenum internalformat, dataformat, type;
    void* data;

    auto rawfile = Common::readFile(filename);

    // Load image from file and read format
    stbi_set_flip_vertically_on_load(true);
    switch (format) {
        case Format::LINEAR8:
        case Format::SRGB8:
        case Format::NORMAL8:
            type = GL_UNSIGNED_BYTE;
            data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(rawfile.c_str()), rawfile.size(), &width, &height, &channels, 0);
            break;
        case Format::FLOAT16:
        case Format::FLOAT32:
            type = GL_FLOAT;
            data = stbi_loadf_from_memory(reinterpret_cast<const stbi_uc*>(rawfile.c_str()), rawfile.size(), &width, &height, &channels, 0);
            break;
        default: assert(false);
    }

    if (!data) throw std::runtime_error("Failed to parse image: " + filename);

    switch(format) {
        case Format::LINEAR8:
            switch (channels) {
                case 1: internalformat = GL_R8; break;
                case 2: internalformat = GL_RG8; break;
                case 3: internalformat = GL_RGB8; break;
                case 4: internalformat = GL_RGBA8; break;
                default: assert(false);
            }
            break;
        case Format::SRGB8:
            switch (channels) {
                case 1: assert(false); break;
                case 2: assert(false); break;
                case 3: internalformat = GL_SRGB8; break;
                case 4: internalformat = GL_SRGB8_ALPHA8; break;
                default: assert(false);
            }
            break;
        case Format::FLOAT16:
            switch (channels) {
                case 1: internalformat = GL_R16F; break;
                case 2: internalformat = GL_RG16F; break;
                case 3: internalformat = GL_RGB16F; break;
                case 4: internalformat = GL_RGBA16F; break;
                default: assert(false);
            }
            break;
        case Format::FLOAT32:
            switch (channels) {
                case 1: internalformat = GL_R32F; break;
                case 2: internalformat = GL_RG32F; break;
                case 3: internalformat = GL_RGB32F; break;
                case 4: internalformat = GL_RGBA32F; break;
                default: assert(false);
            }
            break;
        case Format::NORMAL8:    
            switch (channels) {
                case 1: internalformat = GL_R8_SNORM; break;
                case 2: internalformat = GL_RG8_SNORM; break;
                case 3: internalformat = GL_RGB8_SNORM; break;
                case 4: internalformat = GL_RGBA8_SNORM; break;
                default: assert(false);
            }
            break;
        default: assert(false);
    }

    switch (channels) {
        case 1: dataformat = GL_RED; break;
        case 2: dataformat = GL_RG; break;
        case 3: dataformat = GL_RGB; break;
        case 4: dataformat = GL_RGBA; break;
        default: assert(false);
    }

    // Upload texture data
    bind(Type::TEX2D);
    // This would be the immutable version:
    // Note: On OpenGL 4.5+ one would use the DSA versions glTextureStorage2D and glTextureSubImage2D
    // glTexStorage2D(GL_TEXTURE_2D, mipmaps, internalformat, width, height);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataformat, type, data);
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, dataformat, type, data);

    // Free image data
    stbi_image_free(data);

    // Generate mipmaps
    if (mipmaps > 0) glGenerateMipmap(GL_TEXTURE_2D);
}