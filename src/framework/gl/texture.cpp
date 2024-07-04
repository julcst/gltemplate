#include "texture.hpp"

#include <glbinding/gl46core/gl.h>
using namespace gl46core;

#include <stb_image.h>
#include <stb_image_write.h>

#include <array>
#include <string>
#include <stdexcept>

#include "framework/common.hpp"
#include "framework/context.hpp"

/////////////////////// RAII behavior ///////////////////////
Texture::Texture(GLenum target) : target(target) {
#ifdef MODERN_GL
    glCreateTextures(target, 1, &handle);
#else
    glGenTextures(1, &handle);
#endif
}

Texture::Texture(Texture &&other) noexcept : handle(other.handle), target(other.target) {
    other.handle = 0;
}

Texture &Texture::operator=(Texture &&other) noexcept {
    if (this != &other) {
        release();
        handle = other.handle;
        target = other.target;
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
    glBindTexture(target, handle);
}

void Texture::bindTextureUnit(GLuint index) {
#ifdef MODERN_GL
    // On OpenGL 4.5+ one would use the DSA version glBindTextureUnit
    glBindTextureUnit(index, handle);
#else
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(target, handle);
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
            }
        case Texture::Format::SRGB8:
            switch (channels) {
                case 3: return GL_SRGB8;
                case 4: return GL_SRGB8_ALPHA8;
            }
        case Texture::Format::FLOAT16:
            switch (channels) {
                case 1: return GL_R16F;
                case 2: return GL_RG16F;
                case 3: return GL_RGB16F;
                case 4: return GL_RGBA16F;
            }
        case Texture::Format::FLOAT32:
            switch (channels) {
                case 1: return GL_R32F;
                case 2: return GL_RG32F;
                case 3: return GL_RGB32F;
                case 4: return GL_RGBA32F;
            }
        case Texture::Format::NORMAL8:
            switch (channels) {
                case 1: return GL_R8_SNORM;
                case 2: return GL_RG8_SNORM;
                case 3: return GL_RGB8_SNORM;
                case 4: return GL_RGBA8_SNORM;
            }
    }
    throw std::runtime_error("Unsupported texture format");
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
        default: throw std::runtime_error("Unsupported internal format");
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
        default: throw std::runtime_error("Unsupported internal format");
    }
    return GL_NONE;

}

void Texture::_load2D(GLenum target, Format format, const std::filesystem::path& filepath) {
    GLsizei width, height, channels;
    GLenum dataType;
    void* data;

    // Load image from file and read format
    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    switch (format) {
        case Format::LINEAR8:
        case Format::SRGB8:
        case Format::NORMAL8:
            dataType = GL_UNSIGNED_BYTE;
            data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
            break;
        case Format::FLOAT16:
        case Format::FLOAT32:
            dataType = GL_FLOAT;
            data = stbi_loadf(filepath.string().c_str(), &width, &height, &channels, 0);
            break;
        default: throw std::runtime_error("Unsupported texture format");
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.string() + ": " + stbi_failure_reason());

    GLenum internalFormat = getInternalFormat(format, channels);
    GLenum baseFormat = getBaseFormat(internalFormat);

    // Upload texture data
#ifdef MODERN_GL
    // This would be the immutable version:
    // glTexStorage2D(GL_TEXTURE_2D, levels, internalFormat, width, height);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, baseFormat, dataType, data);
    // Note: On OpenGL 4.5+ one would use the DSA versions glTextureStorage2D and glTextureSubImage2D
    glTextureSubImage2D(handle, 0, 0, 0, width, height, baseFormat, dataType, data);
#else
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
    switch (format) {
        case Format::LINEAR8:
        case Format::SRGB8:
        case Format::NORMAL8:
            dataType = GL_UNSIGNED_BYTE;
            data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
            break;
        case Format::FLOAT16:
        case Format::FLOAT32:
            dataType = GL_FLOAT;
            data = stbi_loadf(filepath.string().c_str(), &width, &height, &channels, 0);
            break;
        default: throw std::runtime_error("Unsupported texture format");
    }

    if (!data) throw std::runtime_error("Failed to parse image " + filepath.string() + ": " + stbi_failure_reason());

    GLenum internalFormat = getInternalFormat(format, channels);
    GLenum baseFormat = getBaseFormat(internalFormat);

    // Upload texture data
#ifdef MODERN_GL
    glTextureSubImage3D(handle, 0, 0, 0, zindex, width, height, 1, baseFormat, dataType, data);
#else
    glTexSubImage3D(target, 0, 0, 0, zindex, width, height, 1, baseFormat, dataType, data);
#endif
}

void Texture::load(Format format, const std::filesystem::path& filepath, GLint mipmaps) {
    stbi_set_flip_vertically_on_load(true); // OpenGL expects the origin to be at the bottom left

#ifdef MODERN_GL
    GLsizei width, height, channels;
    if (!stbi_info(filepath.string().c_str(), &width, &height, &channels))
        throw std::runtime_error("Failed to parse image " + filepath.string() + ": " + stbi_failure_reason());
    GLenum internalFormat = getInternalFormat(format, channels);
    glTextureStorage2D(handle, mipmaps + 1, internalFormat, width, height);
#else
    bind();
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmaps); // Must be set to avoid crashes on some drivers
#endif

    _load2D(target, format, filepath);

    // Generate mipmaps
#ifdef MODERN_GL
    if (mipmaps) glGenerateTextureMipmap(handle);
#else
    if (mipmaps) glGenerateMipmap(target);
#endif
}

void Texture::loadCubemap(Format format, const std::array<std::filesystem::path, 6>& filepaths, GLint mipmaps) {
    // For seamless cubemaps, call glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS)
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    stbi_set_flip_vertically_on_load(false); // Do not flip cubemap faces

#ifdef MODERN_GL
    // Should always be set for cubemaps, see https://www.khronos.org/opengl/wiki_opengl/index.php?title=Common_Mistakes&section=14#Creating_a_Cubemap_Texture
    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    GLsizei width, height, channels;
    if (!stbi_info(filepaths[0].string().c_str(), &width, &height, &channels))
        throw std::runtime_error("Failed to parse image " + filepaths[0].string() + ": " + stbi_failure_reason());
    GLenum internalFormat = getInternalFormat(format, channels);
    glTextureStorage2D(handle, mipmaps + 1, internalFormat, width, height);
    for (int i = 0; i < 6; i++) {
        _load3D(i, format, filepaths[i]);
    }
#else
    bind();
    // Should always be set for cubemaps, see https://www.khronos.org/opengl/wiki_opengl/index.php?title=Common_Mistakes&section=14#Creating_a_Cubemap_Texture
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmaps); // Must be set to avoid crashes on some drivers
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
    if (mipmaps) glGenerateMipmap(target);
#endif
}

void Texture::loadCubemap(Format format, const std::filesystem::path& directory, GLint mipmaps) {
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
    GLint width, height;
    GLenum internalFormat;

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
    GLenum dataType = getType(internalFormat);
    int channels = 4; // glTexImage2D always returns 4 channels

    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_flip_vertically_on_write(true);

    if (dataType == GL_FLOAT) {
        auto floatData = std::make_unique<float[]>(width * height * channels);

    #ifdef MODERN_GL
        glGetTextureImage(handle, 0, baseFormat, dataType, width * height * channels * sizeof(float), floatData.get());
    #else
        glGetTexImage(target, 0, baseFormat, dataType, floatData.get());
    #endif

        return stbi_write_hdr(filepath.string().c_str(), width, height, channels, floatData.get());
    } else if (dataType == GL_UNSIGNED_BYTE) {
        auto byteData = std::make_unique<unsigned char[]>(width * height * channels);

    #ifdef MODERN_GL
        glGetTextureImage(handle, 0, baseFormat, dataType, width * height * channels * sizeof(unsigned char), byteData.get());
    #else
        glGetTexImage(target, 0, baseFormat, dataType, byteData.get());
    #endif
        
        auto ext = filepath.extension();
        if (ext == ".bmp") return stbi_write_bmp(filepath.string().c_str(), width, height, channels, byteData.get());
        else if (ext == ".tga") return stbi_write_tga(filepath.string().c_str(), width, height, channels, byteData.get());
        else if (ext == ".jpg" || ext == ".jpeg") return stbi_write_jpg(filepath.string().c_str(), width, height, channels, byteData.get(), 95);
        else if (ext == ".png") return stbi_write_png(filepath.string().c_str(), width, height, channels, byteData.get(), width * channels);
        return false;
    } else return false;
}

void Texture::set(GLenum parameter, GLenum value) {
#ifdef MODERN_GL
    glTextureParameteri(handle, parameter, value);
#else
    bind();
    glTexParameteri(target, parameter, value);
#endif
}

void Texture::set(GLenum parameter, GLint value) {
#ifdef MODERN_GL
    glTextureParameteri(handle, parameter, value);
#else
    bind();
    glTexParameteri(target, parameter, value);
#endif
}

GLint Texture::get(GLenum parameter, GLint level) {
    GLint value;
#ifdef MODERN_GL
    glGetTextureLevelParameteriv(handle, level, parameter, &value);
#else
    bind();
    glGetTexLevelParameteriv(target, level, parameter, &value);
#endif
    return value;
}