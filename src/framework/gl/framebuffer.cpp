#include "framebuffer.hpp"

#include <unordered_map>

#include <glbinding/gl46core/gl.h>

#include "framework/gl/texture.hpp"

using namespace gl46core;

/////////////////////// RAII behavior ///////////////////////
Framebuffer::Framebuffer() {
#ifdef MODERN_GL
    glCreateFramebuffers(1, &handle);
#else
    glGenFramebuffers(1, &handle);
#endif
}

Framebuffer::Framebuffer(Framebuffer &&other) noexcept : handle(other.handle) {
    other.handle = 0;
}

Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept {
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

void Framebuffer::bind(GLenum target) {
    glBindFramebuffer(target, handle);
}

void Framebuffer::bindDefault(GLenum target) {
    glBindFramebuffer(target, 0);
}

void Framebuffer::attach(GLenum attachment, const Texture& texture, GLint level) {
    attach(attachment, texture.handle, level);
}

void Framebuffer::attach(GLenum attachment, GLuint texture, GLint level) {
#ifdef MODERN_GL
    glNamedFramebufferTexture(handle, attachment, texture, level);
#else
    bind(GL_DRAW_FRAMEBUFFER);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attachment, texture, level);
#endif
}

void Framebuffer::checkStatus(GLenum target) {
#ifdef MODERN_GL
    GLenum status = glCheckNamedFramebufferStatus(handle, target);
#else
    bind(target);
    GLenum status = glCheckFramebufferStatus(target);
#endif

    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            return;
        case GL_FRAMEBUFFER_UNDEFINED:
            throw std::runtime_error("GL_FRAMEBUFFER_UNDEFINED: The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.");
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: Any of the framebuffer attachment points are framebuffer incomplete.");
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: The framebuffer does not have at least one image attached to it.");
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi.");
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.");
        case GL_FRAMEBUFFER_UNSUPPORTED:
            throw std::runtime_error("GL_FRAMEBUFFER_UNSUPPORTED: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or the attached images are a mix of renderbuffers and textures, and the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES.");
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: Any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target.");
        default:
            throw std::runtime_error("Unknown framebuffer status");
    }
}