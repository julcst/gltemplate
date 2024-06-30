#pragma once

#include <glbinding/gl46core/gl.h>
using namespace gl46core;

/**
 * @file vertexarray.hpp
 * @brief Defines a VertexArray class wrapper around the OpenGL vertex array object.
 */

/**
 * @class VertexArray
 * @brief RAII wrapper for a vertex array object in OpenGL.
 * VAOs are used to store vertex attribute configurations and buffer bindings for rendering.
 * See https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object for more information.
 */
class VertexArray {
   public:
    /**
     * @brief Creates a new VertexArray object.
     * The identifier of the OpenGL object is stored in VertexArray::handle.
     */
    VertexArray();

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    VertexArray(const VertexArray&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    VertexArray& operator=(const VertexArray&) = delete;

    /**
     * @brief Move constructor, invalidates the other VertexArray object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The VertexArray object to move from.
     */
    VertexArray(VertexArray&& other);

    /**
     * @brief Move assignment operator, deletes the current VertexArray object and invalidates the other VertexArray object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The VertexArray object to move from.
     * @return A reference to the moved VertexArray object.
     */
    VertexArray& operator=(VertexArray&& other);

    /**
     * @brief Destructor, releases the OpenGL object.
     */
    ~VertexArray();

    /**
     * @brief Binds the vertex array object.
     */
    void bind();

    /**
     * @brief The unique handle that identifies the vertex array object on the GPU.
     */
    GLuint handle;

   private:
    /**
     * @brief Releases the resources associated with the VertexArray object.
     * 
     * This function is called by the destructor and move assignment operator to release the resources
     * associated with the VertexArray object.
     */
    void release();
};