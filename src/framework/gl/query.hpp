#pragma once

#include <glad/gl.h>

/**
 * @file query.hpp
 * @brief Defines a Query class wrapper around the OpenGL query object.
 */

/**
 * @class Query
 * @brief RAII wrapper for OpenGL query with helper functions for timing queries.
 * See https://www.khronos.org/opengl/wiki/Query_Object for more information.
 */
class Query {
   public:
    /**
     * @brief Constructs a Query object.
     * The identifier of the OpenGL object is stored in `Query::handle`.
     */
    Query();

    /**
     * @brief Copy constructor (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Query(const Query&) = delete;

    /**
     * @brief Copy assignment operator (deleted).
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     */
    Query& operator=(const Query&) = delete;

    /**
     * @brief Move constructor, invalidates the other Query object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Query object to move from.
     */
    Query(Query &&other) noexcept;

    /**
     * @brief Move assignment operator, deletes the current Query object and invalidates the other Query object.
     * (RAII idiom, see https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls)
     * @param other The Query object to move from.
     * @return A reference to the moved Query object.
     */
    Query &operator=(Query &&other) noexcept;

    /**
     * @brief Destructor, frees the OpenGL object.
     */
    ~Query();

    /**
     * @brief Begins the query for the specified target.
     * 
     * @param target The target for which the query should begin, e.g. `GL_TIME_ELAPSED`.
     */
    void begin(GLenum target);

    /**
     * @brief Ends the query for the specified target and returns the `GL_QUERY_RESULT` as unsigned integer.
     * This can be used e.g. for timing queries (`GL_TIME_ELAPSED`).
     * 
     * @param target The target for which the query should end, e.g. `GL_TIME_ELAPSED`.
     * @return The result of the query as unsigned integer.
     */
    GLuint end(GLenum target);

    /**
     * @brief The unique handle that identifies the query object on the GPU.
     */
    GLuint handle;

   private:
    /**
     * @brief Releases the resources associated with the query.
     */
    void release();
};