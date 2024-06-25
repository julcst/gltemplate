#pragma once

#include "gl/buffer.hpp"

template <typename T>
class UniformBuffer {
   public:
    UniformBuffer(unsigned int index, const T& uniforms = T{});
    void upload(const T& uniforms);
    void bind(unsigned int index);

    Buffer buffer;
};

template <typename T>
UniformBuffer<T>::UniformBuffer(unsigned int index, const T& uniforms) : buffer(GL_UNIFORM_BUFFER) {
    buffer.bind(index);
    buffer.load(uniforms);
}

template <typename T>
void UniformBuffer<T>::upload(const T& uniforms) {
    buffer.set(uniforms);
}

template <typename T>
void UniformBuffer<T>::bind(unsigned int index) {
    buffer.bind(index);
}