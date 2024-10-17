#include "framebuffer.hpp"

#include <unordered_map>

#include <glad/gl.h>

#include "framework/gl/texture.hpp"

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

bool Framebuffer::writeToFile(const std::filesystem::path &path, GLenum attachment) {
    GLsizei width, height;
    GLint type, internalFormat;
    GLint texture;
#ifdef MODERN_GL
    glNamedFramebufferReadBuffer(handle, attachment);
    glGetNamedFramebufferAttachmentParameteriv(handle, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
    glGetNamedFramebufferAttachmentParameteriv(handle, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture);
    switch (type) {
        case GL_TEXTURE:
            glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_WIDTH, &width);
            glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_HEIGHT, &height);
            glGetTextureLevelParameteriv(texture, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
            break;
        case GL_RENDERBUFFER:
            glGetNamedRenderbufferParameteriv(texture, GL_RENDERBUFFER_WIDTH, &width);
            glGetNamedRenderbufferParameteriv(texture, GL_RENDERBUFFER_HEIGHT, &height);
            glGetNamedRenderbufferParameteriv(texture, GL_RENDERBUFFER_INTERNAL_FORMAT, &internalFormat);
            break;
        default:
            throw std::runtime_error("Unknown framebuffer attachment type");
    }
#else
    glBindFramebuffer(GL_READ_FRAMEBUFFER, handle);
    glReadBuffer(attachment);
    glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
    glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture);
    switch (type) {
        case GL_TEXTURE:
            glBindTexture(GL_TEXTURE_2D, texture);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
            break;
        case GL_RENDERBUFFER:
            glBindRenderbuffer(GL_RENDERBUFFER, texture);
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &internalFormat);
            break;
        default:
            throw std::runtime_error("Unknown framebuffer attachment type");
    }
#endif
    GLenum dataType = getDataType(internalFormat);
    GLenum baseFormat = getBaseFormat(internalFormat);
    int channels = getChannels(baseFormat);

    Context::setWorkingDirectory(); // Ensure the working directory is set correctly
    stbi_flip_vertically_on_write(true);

    if (dataType == GL_UNSIGNED_BYTE || dataType == GL_BYTE) {
        auto ubyteData = std::make_unique<unsigned char[]>(width * height * channels);
        glReadPixels(0, 0, width, height, baseFormat, dataType, ubyteData.get());

        // Convert from signed to unsigned byte
        if (dataType == GL_BYTE)
            for (int i = 0; i < width * height * channels; i++)
                ubyteData[i] += 128;

        auto ext = path.extension();
        if (ext == ".png")
            return stbi_write_png(path.string().c_str(), width, height, channels, ubyteData.get(), width * channels);
        else if (ext == ".bmp")
            return stbi_write_bmp(path.string().c_str(), width, height, channels, ubyteData.get());
        else if (ext == ".tga")
            return stbi_write_tga(path.string().c_str(), width, height, channels, ubyteData.get());
        else if (ext == ".jpg" || ext == ".jpeg")
            return stbi_write_jpg(path.string().c_str(), width, height, channels, ubyteData.get(), 95);
        else
            throw std::runtime_error("Unsupported image format");
    } else if (dataType == GL_FLOAT) {
        auto floatData = std::make_unique<float[]>(width * height * channels);
        glReadPixels(0, 0, width, height, baseFormat, dataType, floatData.get());
        return stbi_write_hdr(path.string().c_str(), width, height, channels, floatData.get());
    } else throw std::runtime_error("Unsupported data type");
}