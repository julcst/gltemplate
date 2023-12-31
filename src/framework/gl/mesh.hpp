#pragma once

#include <vector>

#include "buffer.hpp"
#include "vertexarray.hpp"

const std::vector<float> FULLSCREEN_VERTICES = {
    -1.0f, -1.0f, 0.0f,
    3.0f, -1.0f, 0.0f,
    -1.0f,  3.0f, 0.0f,
};

const std::vector<unsigned int> FULLSCREEN_INDICES = {
    0, 1, 2,
};

// TODO: Check if RAII behavior is inherited
class Mesh : public VertexArray {
   public:
    void load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void draw();

   private:
    unsigned int numVertices, numIndices = 0;
    Buffer vbo;
    Buffer ebo;
};
