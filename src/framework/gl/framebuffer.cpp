#include "framebuffer.hpp"

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>
#include <iostream>

#include "framework/gl/texture.hpp"

/////////////////////// RAII behavior ///////////////////////
Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &handle);
    assert(handle);
}

Framebuffer::Framebuffer(Framebuffer&& other) : handle(other.handle) {
    other.handle = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) {
    if (this != &other) {
        release();
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

Framebuffer::~Framebuffer() {
    release();
}

void Framebuffer::release() {
    if (handle) glDeleteFramebuffers(1, &handle);
}
/////////////////////////////////////////////////////////////

void Framebuffer::bind(Type type) {
    glBindFramebuffer(static_cast<GLenum>(type), handle);
}

void Framebuffer::bindDefault(Type type) {
    glBindFramebuffer(static_cast<GLenum>(type), 0);
}

void Framebuffer::attach(Type type, Attachment attachment, Texture texture, GLint level) {
    bind(type);
    glFramebufferTexture(static_cast<GLenum>(type), static_cast<GLenum>(attachment), texture.handle, level);
    attachments[attachment] = std::move(texture);
}

void Framebuffer::attach(Type type, Attachment attachment, GLuint texture, GLint level) {
    bind(type);
    glFramebufferTexture(static_cast<GLenum>(type), static_cast<GLenum>(attachment), texture, level);
}

bool Framebuffer::checkStatus(Type type) {
    bind(type);
    GLenum status = glCheckFramebufferStatus(static_cast<GLenum>(type));
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            return true;
        case GL_FRAMEBUFFER_UNDEFINED:
            std::cerr << "GL_FRAMEBUFFER_UNDEFINED is returned if the specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist." << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT is returned if any of the framebuffer attachment points are framebuffer incomplete." << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT is returned if the framebuffer does not have at least one image attached to it." << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER is returned if GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is returned if the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES. \n\nGL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE is also returned if the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS is returned if any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target." << std::endl;
            break;
        default:
            std::cerr << "Framebuffer incomplete: Unknown error" << std::endl;
            break;
    }
    return false;
}