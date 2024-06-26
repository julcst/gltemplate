#pragma once

#include <glbinding/gl/gl.h>
using namespace gl;

#include <vector>

/**
 * @file buffer.hpp
 * @brief Defines a Buffer class wrapper around the OpenGL buffer object.
 */

/**
 * @class Buffer
 * @brief RAII wrapper for OpenGL buffer with helper functions for loading and setting data.
 * See https://www.khronos.org/opengl/wiki/Buffer_Object for more information.
 */
class Buffer {
   public:
    /**
     * @brief Creates a new Buffer object.
     * The identifier of the OpenGL object is stored in `Buffer::handle`.
     * Initialize a Buffer object like this in the header file: `Buffer buffer { GL_ARRAY_BUFFER };`
     * @param target The target/type for which the buffer should be created, e.g. `GL_ARRAY_BUFFER`.
     */
    Buffer(GLenum target);

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Buffer(const Buffer&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Buffer& operator=(const Buffer&) = delete;
    
    /**
     * @brief Move constructor, invalidates the other Buffer object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Buffer object to move from.
     */
    Buffer(Buffer&& other);

    /**
     * @brief Move assignment operator, deletes the current Buffer object and invalidates the other Buffer object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Buffer object to move from.
     * @return A reference to the moved Buffer object.
     */
    Buffer& operator=(Buffer&& other);

    /**
     * @brief Destructor, releases the OpenGL object.
     */
    ~Buffer();

    /**
     * @brief Binds the buffer object to the specified target.
     */
    void bind();

    /**
     * @brief Binds the buffer object to the specified index.
     * This is used for binding the buffer to a specific index in a shader, for example for uniform buffer objects and shader storage buffer objects.
     * @param index The index to bind the buffer to.
     */
    void bind(GLuint index);

    /**
     * @brief Loads data into the buffer object.
     * This function allocates mutable memory on the GPU and must be called before using set.
     * @param size The size of the data to load into the buffer object.
     * @param data The pointer to the data to load into the buffer object.
     * @param usage The usage pattern of the data, e.g. `GL_STATIC_DRAW`, `GL_DYNAMIC_DRAW`, `GL_STREAM_DRAW` (See https://www.khronos.org/opengl/wiki/Buffer_Object#Usage_patterns).
     */
    void _load(GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW);

    /**
     * @brief Loads data as an array of a custom struct into the buffer object.
     * This function allocates mutable memory on the GPU and must be called before using `Buffer::set`.
     * @param data The data to load into the buffer object.
     * @tparam T The type of the data to load into the buffer object. Please refer https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout to ensure the data is correctly aligned.
     * In short, the data should be aligned to multiples of 16 bytes, which is one vec4 or 4 floats.
     * @param usage The usage pattern of the data, e.g. `GL_STATIC_DRAW`, `GL_DYNAMIC_DRAW`, `GL_STREAM_DRAW` (See https://www.khronos.org/opengl/wiki/Buffer_Object#Usage_patterns).
     */
    template <typename T>
    void load(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW);

    /**
     * @brief Loads a single element into the buffer object.
     * This function allocates mutable memory on the GPU and must be called before using `Buffer::set`.
     * @param data The data to load into the buffer object.
     * @tparam T The type of the data to load into the buffer object. Please refer https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout to ensure the data is correctly aligned.
     * In short, the data should be aligned to multiples of 16 bytes, which is one vec4 or 4 floats.
     * @param usage The usage pattern of the data, e.g. `GL_STATIC_DRAW`, `GL_DYNAMIC_DRAW`, `GL_STREAM_DRAW` (See https://www.khronos.org/opengl/wiki/Buffer_Object#Usage_patterns).
     */
    template <typename T>
    void load(const T& data, GLenum usage = GL_STATIC_DRAW);

    /**
     * @brief Sets data into the buffer object.
     * This function requires the buffer object to have been previously allocated with `Buffer::load`.
     * @param size The size of the data to set into the buffer object.
     * @param data The pointer to the data to set into the buffer object.
     * @param offset The offset in the buffer object to set the data.
     */
    void _set(GLsizeiptr size, const GLvoid* data, GLintptr offset);

    /**
     * @brief Sets data as an array of a custom struct into the buffer object.
     * This function requires the buffer object to have been previously allocated with `Buffer::load`.
     * @param data The data to set into the buffer object.
     * @param offset The offset in the buffer object to set the data.
     * @tparam T The type of the data to set into the buffer object. Please refer https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout to ensure the data is correctly aligned.
     * In short, the data should be aligned to multiples of 16 bytes, which is one vec4 or 4 floats.
     */
    template <typename T>
    void set(const std::vector<T>& data, unsigned int offset = 0);

    /**
     * @brief Sets a single element into the buffer object.
     * This function requires the buffer object to have been previously allocated with `Buffer::load`.
     * @param data The data to set into the buffer object.
     * @param offset The offset in the buffer object to set the data.
     * @tparam T The type of the data to set into the buffer object. Please refer https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)#Memory_layout to ensure the data is correctly aligned.
     * In short, the data should be aligned to multiples of 16 bytes, which is one vec4 or 4 floats.
     */
    template <typename T>
    void set(const T& data, unsigned int offset = 0);

    /**
     * @brief Allocates memory on the GPU for the buffer object.
     * @param size The size of the memory to allocate on the GPU.
     * @param usage The usage pattern of the data, e.g. `GL_STATIC_DRAW`, `GL_DYNAMIC_DRAW`, `GL_STREAM_DRAW` (See https://www.khronos.org/opengl/wiki/Buffer_Object#Usage_patterns).
     */
    void allocate(GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);

    /**
     * @brief The unique handle that identifies the buffer object on the GPU.
     */
    GLuint handle;

    /**
     * @brief The target/type of the buffer object, e.g. `GL_ARRAY_BUFFER`.
     */
    GLenum target;

   private:
    void release();
};

template <typename T>
inline void Buffer::load(const std::vector<T>& data, GLenum usage) {
    _load(sizeof(T) * data.size(), data.data(), usage);
}

template <typename T>
inline void Buffer::load(const T& data, GLenum usage) {
    _load(sizeof(T), &data, usage);
}

template <typename T>
inline void Buffer::set(const std::vector<T>& data, unsigned int offset) {
    _set(sizeof(T) * data.size(), data.data(), offset);
}

template <typename T>
inline void Buffer::set(const T& data, unsigned int offset) {
    _set(sizeof(T), &data, offset);
}