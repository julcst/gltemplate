#pragma once

#include <glad/gl.h>

#include <filesystem>

#include "framework/gl/texture.hpp"

/**
 * @file framebuffer.hpp
 * @brief Defines a Framebuffer class wrapper around the OpenGL framebuffer object.
 */

/**
 * @class Framebuffer
 * @brief RAII wrapper for OpenGL framebuffer with helper functions for attaching textures.
 * See https://www.khronos.org/opengl/wiki/Framebuffer_Object for more information.
 */
class Framebuffer {
   public:
    /**
     * @brief The default framebuffer object (0) used for rendering to the window.
     */
    static Framebuffer getDefault() {
        return Framebuffer(0);
    }

    /**
     * @brief Constructs a Framebuffer object.
     * The identifier of the OpenGL object is stored in `Framebuffer::handle`.
     */
    Framebuffer();

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Framebuffer(const Framebuffer&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Framebuffer& operator=(const Framebuffer&) = delete;
    
    /**
     * @brief Move constructor, invalidates the other Framebuffer object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Framebuffer object to move from.
     */
    Framebuffer(Framebuffer &&other) noexcept;

    /**
     * @brief Move assignment operator, deletes the current Framebuffer object and invalidates the other Framebuffer object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Framebuffer object to move from.
     * @return A reference to the moved Framebuffer object.
     */
    Framebuffer &operator=(Framebuffer &&other) noexcept;

    /**
     * @brief Destructor, releases the OpenGL object.
     */
    ~Framebuffer();

    /**
     * @brief Binds the framebuffer object.
     * @param target What to bind the framebuffer for, one of `GL_FRAMEBUFFER`, `GL_READ_FRAMEBUFFER`, or `GL_DRAW_FRAMEBUFFER`.
     */
    void bind(GLenum target = GL_FRAMEBUFFER);

    /**
     * @brief Binds the default framebuffer object (0) used for rendering to the window.
     * @param target What to bind the framebuffer for, one of `GL_FRAMEBUFFER`, `GL_READ_FRAMEBUFFER`, or `GL_DRAW_FRAMEBUFFER`.
     */
    static void bindDefault(GLenum target = GL_FRAMEBUFFER);

    /**
     * @brief Writes a color attachment of the framebuffer to a file.
     * @param path The path to write the image to.
     * @param attachment The attachment point, e.g. `GL_COLOR_ATTACHMENT0`, `GL_DEPTH_ATTACHMENT`, ...
     */
    bool writeToFile(const std::filesystem::path& path, GLenum attachment = GL_COLOR_ATTACHMENT0);

    /**
     * @brief Attaches a texture to the framebuffer.
     * @param attachment The attachment point, e.g. `GL_COLOR_ATTACHMENT0`, `GL_DEPTH_ATTACHMENT`, `GL_STENCIL_ATTACHMENT`.
     * @param texture The texture to attach.
     * @param level The mipmap level to attach.
     */
    template<GLenum target>
    void attach(GLenum attachment, const Texture<target>& texture, GLint level = 0);

    /**
     * @brief Attaches a texture to the framebuffer.
     * @param attachment The attachment point, e.g. `GL_COLOR_ATTACHMENT0`, `GL_DEPTH_ATTACHMENT`, `GL_STENCIL_ATTACHMENT`.
     * @param texture The texture to attach.
     * @param level The mipmap level to attach.
     */
    void attach(GLenum attachment, GLuint texture, GLint level = 0);

    /**
     * @brief Checks the status of the framebuffer.
     * @throws std::runtime_error if the framebuffer is incomplete.
     */
    void checkStatus(GLenum target = GL_FRAMEBUFFER);

    /**
     * @brief The unique handle that identifies the framebuffer object on the GPU.
     */
    GLuint handle;

   private:
    /**
     * @brief  This function is called by the destructor and move assignment operator to release the resources associated with the Framebuffer object.
     */
    void release();

    Framebuffer(GLuint handle) : handle(handle) {} // Private constructor for DEFAULT framebuffer
};

template <GLenum target>
void Framebuffer::attach(GLenum attachment, const Texture<target>& texture, GLint level) {
    attach(attachment, texture.handle, level);
}