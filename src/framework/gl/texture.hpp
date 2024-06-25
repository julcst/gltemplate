#pragma once

#include <array>
#include <filesystem>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>

using namespace gl;

/**
 * RAII wrapper for OpenGL texture
 * Our framework uses mutable textures because immutable textures are not available in OpenGL 4.1
 * Mutable texture creation with glTexImage2D though should be avoided because it is slower and more error-prone
 */
class Texture {
   public:
    enum class Format {     // See https://www.khronos.org/opengl/wiki/Image_Format
        LINEAR8,            // 8-bit unsigned normalized integer
        NORMAL8,            // 8-bit signed normalized integer
        SRGB8,              // 8-bit sRGB with linear alpha
        FLOAT16,            // 16-bit floating point
        FLOAT32,            // 32-bit floating point
        DEPTH32F_STENCIL8,  // 32-bit floating point depth, 8-bit stencil
    };

    Texture(GLenum target);
    // Disable copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    // Implement moving
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);
    ~Texture();
    void bind();
    void bindTextureUnit(GLuint index);
    void _load2D(GLenum target, Format format, const std::filesystem::path& filepath);
    void _load3D(GLint zindex, Format format, const std::filesystem::path& filepath);
    void load(Format format, const std::filesystem::path& filepath, bool mipmaps = false);
    void loadCubemap(Format format, const std::array<std::filesystem::path, 6>& filepaths, bool mipmaps = false);
    void loadCubemap(Format format, const std::filesystem::path& directory, bool mipmaps = false);
    bool writeToFile(const std::filesystem::path& filepath);
    void set(GLenum parameter, GLint value);
    int get(GLenum parameter, GLint level);

    GLuint handle;
    GLenum target;

   private:
    void release();
};