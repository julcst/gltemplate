#pragma once

#include "vertexarray.hpp"
#include "buffer.hpp"

#include <vector>

class Mesh : VertexArray {
public:
    void load(std::vector<float> vertices, std::vector<unsigned int> indices);
    void draw();
private:
    unsigned int numVertices, numIndices = 0;
    Buffer vbo;
    Buffer ebo;
};
