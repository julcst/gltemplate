#include "mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <objgl.h>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "common.hpp"
#include "config.hpp"

using namespace glm;

void Mesh::load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numVertices = vertices.size();
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    vao.unbind();
}

void Mesh::load(const std::string& filepath) {
    // read file
    std::string rawobj = Common::readFile(filepath);
    path = filepath;

    // parse
    ObjGLData model = objgl_loadObj(rawobj.c_str());

    numVertices = model.numVertices;
    numIndices = model.numIndices;

    // load buffers
    vbo._load(Buffer::Type::ARRAY_BUFFER, numVertices * model.vertSize, model.data);
    ebo._load(Buffer::Type::INDEX_BUFFER, numIndices * sizeof(uint_least32_t), model.indices);

    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);

    // vertex atributes
    // location 0 position
    // location 1 texture coordinates
    // location 2 normals
    if (model.hasNormals && !model.hasTexCoords) {
        // positions and normals
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);
    } else if (!model.hasNormals && model.hasTexCoords) {
        // positions and texture coordinates
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    } else if (model.hasNormals && model.hasTexCoords) {
        // positons, normals and texture coordinates
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    } else {
        // only positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    vao.unbind();

    // cleanup
    objgl_delete(&model);
}

struct Vertex {
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
};

void addTangentsToVertex(Vertex& v0, const Vertex& v1, const Vertex& v2) {
    vec3 delta_pos1 = v1.position - v0.position;
    vec3 delta_pos2 = v2.position - v0.position;

    vec2 delta_uv1 = v1.texCoord - v0.texCoord;
    vec2 delta_uv2 = v2.texCoord - v0.texCoord;

    // From: https://sotrh.github.io/learn-wgpu/showcase/compute
    float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
    v0.tangent += (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
    v0.bitangent += (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r; 
}

bool Mesh::loadWithTangents(const std::string& filepath) {
    // Read file
    std::string rawobj = Common::readFile(filepath);
    path = filepath;

    // Parse
    ObjGLData model = objgl_loadObj(rawobj.c_str());
    if (!model.hasNormals || !model.hasTexCoords) return false;
    numVertices = model.numVertices;
    numIndices = model.numIndices;

    // Generate tangents and bitangents
    std::vector<Vertex> vertices(numVertices);
    // TODO: Maybe remove this
    std::vector<float> triangleCount(numVertices);

    // Read vertex data from model
    const uint floatsPerSrcVertex = model.vertSize / sizeof(float);
    for (uint i = 0; i < numVertices; i++) {
        uint srcIndex = i * floatsPerSrcVertex;
        Vertex v;
        v.position  = vec3(model.data[srcIndex + 0u], model.data[srcIndex + 1u], model.data[srcIndex + 2u]);
        v.texCoord  = vec2(model.data[srcIndex + 3u], model.data[srcIndex + 4u]);
        v.normal    = vec3(model.data[srcIndex + 5u], model.data[srcIndex + 6u], model.data[srcIndex + 7u]);
        v.tangent   = vec3(0.0f);
        v.bitangent = vec3(0.0f);
        vertices[i] = v;
        triangleCount[i] = 0.0f;
    }

    // Loop over triangles
    for (uint i = 0; i < numIndices; i += 3) {
        // Get vertices
        uint index0 = model.indices[i + 0u];
        uint index1 = model.indices[i + 1u];
        uint index2 = model.indices[i + 2u];
        Vertex& v0 = vertices[index0];
        Vertex& v1 = vertices[index1];
        Vertex& v2 = vertices[index2];

        vec3 delta_pos1 = v1.position - v0.position;
        vec3 delta_pos2 = v2.position - v0.position;

        vec2 delta_uv1 = v1.texCoord - v0.texCoord;
        vec2 delta_uv2 = v2.texCoord - v0.texCoord;

        float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
        vec3 tangent = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
        vec3 bitangent = (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

        v0.tangent += tangent; v0.bitangent += bitangent;
        v1.tangent += tangent; v1.bitangent += bitangent;
        v2.tangent += tangent; v2.bitangent += bitangent;

        /*// Add tangent and bitangent to each vertex
        addTangentsToVertex(v0, v1, v2);
        addTangentsToVertex(v1, v2, v0);
        addTangentsToVertex(v2, v0, v1);*/

        // Increment the triangle count for each vertex
        triangleCount[index0]++;
        triangleCount[index1]++;
        triangleCount[index2]++;
    }

    // Average tangents and bitangents per vertex
    for (uint i = 0; i < numVertices; i++) {
        if (triangleCount[i] > 0.0f) {
            vertices[i].tangent /= triangleCount[i]; // This should not be necessary because we normalize in the next step
            vertices[i].bitangent /= triangleCount[i]; // This should not be necessary because we normalize in the next step
            //vertices[i].tangent = normalize(vertices[i].tangent);
            //vertices[i].bitangent = normalize(vertices[i].bitangent);
        }
    }

    // Load buffers
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo._load(Buffer::Type::INDEX_BUFFER, numIndices * sizeof(uint_least32_t), model.indices);

    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);

    // Vertex attributes
    // location 0 vec3 position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0 * sizeof(float)));
    // location 1 vec2 texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // location 2 vec3 normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
    // location 3 vec3 tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
    // location 4 vec3 bitangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(11 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // Cleanup
    vao.unbind();
    objgl_delete(&model);
    return true;
}

void Mesh::draw() {
    vao.bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    vao.unbind();
}

std::string Mesh::getFilepath() {
    return path;
}