#pragma once

#include <glad/glad.h>

#include <unordered_map>

#include "framework/gl/texture.hpp"

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
    void bind(GLenum type = Type::READ_AND_DRAW);
    static void bindDefault(GLenum type = Type::READ_AND_DRAW);
    void attach(GLenum type, Attachment attachment, Texture texture, GLint level = 0);
    void attach(GLenum type, Attachment attachment, GLuint texture, GLint level = 0);
    bool checkStatus(GLenum type = Type::READ_AND_DRAW);

    GLuint handle;
    std::unordered_map<Attachment, Texture> attachments;

   private:
    void release();
};