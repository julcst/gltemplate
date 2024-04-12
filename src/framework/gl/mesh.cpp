#include "mesh.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <tiny_obj_loader.h>

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "common.hpp"
#include "config.hpp"

using namespace glm;


////////////////////////// Manual mesh loading //////////////////////////

void Mesh::load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    size_t stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    vao.unbind();
}

void Mesh::load(const std::vector<VertexPCN>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    // Vertex attributes
    size_t stride = sizeof(VertexPCN);
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    vao.unbind();
}

void Mesh::load(const std::vector<VertexPCNT>& vertices, const std::vector<unsigned int>& indices) {
    // Load data into buffers
    numIndices = indices.size();
    vbo.load(Buffer::Type::ARRAY_BUFFER, vertices);
    ebo.load(Buffer::Type::INDEX_BUFFER, indices);

    // Bind buffers to VAO
    // TODO: Use DSA instead (but only OpenGL 4.5+, so not on macOS)
    vao.bind();
    vbo.bind(Buffer::Type::ARRAY_BUFFER);
    ebo.bind(Buffer::Type::INDEX_BUFFER);
    // Vertex attributes
    size_t stride = sizeof(VertexPCNT);
    // (location = 0) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) (0 * sizeof(float)));
    // (location = 1) uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    // (location = 2) normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (5 * sizeof(float)));
    // (location = 3) tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*) (8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    vao.unbind();
}

////////////////////// Obj loading without tangents //////////////////////

/* Define comparison operator for use with std::unordered_map */
bool operator==(const Mesh::VertexPCN& v1, const Mesh::VertexPCN& v2) {
    return v1.position == v2.position && v1.texCoord == v2.texCoord && v1.normal == v2.normal;
}

/* Define hash function for use with std::unordered_map */
template<>
struct std::hash<Mesh::VertexPCN>
{
    std::size_t operator()(const Mesh::VertexPCN& vertex) const noexcept
    {
        size_t seed = 0;
        Common::hash_combine(seed, vertex.position, vertex.texCoord, vertex.normal);
        return seed;
    }
};

void Mesh::load(const std::string& filepath) {
    // Parse OBJ file
    std::string rawobj = Common::readFile(filepath);
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    if (!reader.ParseFromString(rawobj, "", reader_config))
        throw std::runtime_error("Failed to load OBJ file \"" + filepath + "\": " + reader.Error());
    if (!reader.Warning().empty())
        std::cout << "Warning loading OBJ file \"" << filepath << "\": " << reader.Warning() << std::endl;
    
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    std::unordered_map<VertexPCN, uint32_t> uniqueVertices{};
    std::vector<VertexPCN> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VertexPCN vertex{};
            
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    load(vertices, indices);
}

/////////////////////// Obj loading with tangents ///////////////////////

/* Define comparison operator for use with std::unordered_map */
bool operator==(const Mesh::VertexPCNT& v1, const Mesh::VertexPCNT& v2) {
    return v1.position == v2.position && v1.texCoord == v2.texCoord && v1.normal == v2.normal;
}

/* Define hash function for use with std::unordered_map */
template<>
struct std::hash<Mesh::VertexPCNT>
{
    std::size_t operator()(const Mesh::VertexPCNT& vertex) const noexcept
    {
        size_t seed = 0;
        Common::hash_combine(seed, vertex.position, vertex.texCoord, vertex.normal);
        return seed;
    }
};

void Mesh::loadWithTangents(const std::string& filepath) {
    // Parse OBJ file
    std::string rawobj = Common::readFile(filepath);
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;
    if (!reader.ParseFromString(rawobj, "", reader_config))
        throw std::runtime_error("Failed to load OBJ file \"" + filepath + "\": " + reader.Error());
    if (!reader.Warning().empty())
        std::cout << "Warning loading OBJ file \"" << filepath << "\": " << reader.Warning() << std::endl;
    
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    std::unordered_map<VertexPCNT, uint32_t> uniqueVertices{};
    std::vector<VertexPCNT> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VertexPCNT vertex{};
            
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            vertex.tangent = vec3(0.0f);

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    std::vector<float> triangleCount(vertices.size());

    // Loop over triangles
    for (uint i = 0; i < indices.size(); i += 3) {
        // Get triangle vertices
        uint index0 = indices[i + 0u];
        uint index1 = indices[i + 1u];
        uint index2 = indices[i + 2u];
        VertexPCNT& v0 = vertices[index0];
        VertexPCNT& v1 = vertices[index1];
        VertexPCNT& v2 = vertices[index2];

        vec3 delta_pos1 = v1.position - v0.position;
        vec3 delta_pos2 = v2.position - v0.position;

        vec2 delta_uv1 = v1.texCoord - v0.texCoord;
        vec2 delta_uv2 = v2.texCoord - v0.texCoord;

        float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
        vec3 tangent = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;

        // Increment the triangle count for each vertex
        triangleCount[index0]++;
        triangleCount[index1]++;
        triangleCount[index2]++;
    }

    // Average tangents and bitangents per vertex
    for (uint i = 0; i < vertices.size(); i++) {
        if (triangleCount[i] > 0.0f) vertices[i].tangent /= triangleCount[i];
    }

    load(vertices, indices);
}

///////////////////////////// Mesh drawing /////////////////////////////

void Mesh::draw() {
    vao.bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    vao.unbind();
}