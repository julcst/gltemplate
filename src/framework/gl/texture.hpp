#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

/**
 * RAII wrapper for OpenGL texture
 * Our framework uses mutable textures because immutable textures are not available in OpenGL 4.1
 * Mutable texture creation with glTexImage2D though should be avoided because it is slower and more error-prone
 */
class Texture {
   public:
    enum class Type {
        TEX1D = GL_TEXTURE_1D,
        TEX2D = GL_TEXTURE_2D,
        TEX3D = GL_TEXTURE_3D,
    };
    enum class Format {
        LINEAR8,
        SRGB8,
        FLOAT16,
        NORMAL8,
    };
    Texture();
    // Disable copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    // Implement moving
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);
    ~Texture();
    void bind(Type type);
    void bind(Type type, GLuint index);
    void load(Format format, const std::string& filename, GLsizei mipmaps);

   private:
    GLuint handle;
    void release();
};