#pragma once

#include <glad/glad.h>

#include <unordered_map>

#include "framework/gl/texture.hpp"

/**
 * RAII wrapper for OpenGL Framebuffer
 */
class Framebuffer {
   public:
    enum class Type {
        READ_AND_DRAW = GL_FRAMEBUFFER,
        READ = GL_READ_FRAMEBUFFER,
        DRAW = GL_DRAW_FRAMEBUFFER,
    };
    enum class Attachment {
        COLOR0 = GL_COLOR_ATTACHMENT0,
        COLOR1 = GL_COLOR_ATTACHMENT1,
        COLOR2 = GL_COLOR_ATTACHMENT2,
        COLOR3 = GL_COLOR_ATTACHMENT3,
        COLOR4 = GL_COLOR_ATTACHMENT4,
        COLOR5 = GL_COLOR_ATTACHMENT5,
        COLOR6 = GL_COLOR_ATTACHMENT6,
        COLOR7 = GL_COLOR_ATTACHMENT7,
        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
    };

    Framebuffer();
    // Disable copying
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    // Implement moving
    Framebuffer(Framebuffer&& other);
    Framebuffer& operator=(Framebuffer&& other);
    ~Framebuffer();
    void bind(Type type = Type::READ_AND_DRAW);
    static void bindDefault(Type type = Type::READ_AND_DRAW);
    void attach(Type type, Attachment attachment, Texture texture, GLint level = 0);
    void attach(Type type, Attachment attachment, GLuint texture, GLint level = 0);
    bool checkStatus(Type type = Type::READ_AND_DRAW);

    GLuint handle;
    std::unordered_map<Attachment, Texture> attachments;

   private:
    void release();
};