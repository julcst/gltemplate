#pragma once

#include <string>
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

class Mesh {
   public:
    void load(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void load(const std::string& filepath);
    bool loadWithTangents(const std::string& filepath);
    void draw();
    std::string getFilepath();

   private:
    unsigned int numVertices, numIndices = 0;
    VertexArray vao;
    Buffer vbo;
    Buffer ebo;
    std::string path;
};
