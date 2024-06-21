#pragma once

#include <glbinding/gl/gl.h>

#include "framework/gl/texture.hpp"

using namespace gl;

/**
 * RAII wrapper for OpenGL Framebuffer
 */
class Framebuffer {
   public:
    Framebuffer();
    // Disable copying
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    // Implement moving
    Framebuffer(Framebuffer&& other);
    Framebuffer& operator=(Framebuffer&& other);
    ~Framebuffer();
    void bind(GLenum type = GL_FRAMEBUFFER);
    static void bindDefault(GLenum type = GL_FRAMEBUFFER);
    void attach(GLenum type, GLenum attachment, Texture texture, GLint level = 0);
    void attach(GLenum type, GLenum attachment, GLuint texture, GLint level = 0);
    bool checkStatus(GLenum type = GL_FRAMEBUFFER);

    GLuint handle;

   private:
    void release();
};