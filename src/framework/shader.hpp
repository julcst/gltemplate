#pragma once

#include <glm/glm.hpp>

#include <string>

class Shader {
public:
    Shader(std::string filename, unsigned int type);
    ~Shader();
    void compile();
    unsigned int getHandle();
private:
    unsigned int shader;
};

class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(std::string vertexShader, std::string fragmentShader);
    ~ShaderProgram();
    void attach(Shader shader);
    void link();
    void bind();
    unsigned int uniform(std::string name);
    void set(unsigned int loc, int value);
    void set(unsigned int loc, float value);
    void set(unsigned int loc, glm::ivec2 value);
    void set(unsigned int loc, glm::vec2 value);
    void set(unsigned int loc, glm::ivec3 value);
    void set(unsigned int loc, glm::vec3 value);
private:
    unsigned int program;
};