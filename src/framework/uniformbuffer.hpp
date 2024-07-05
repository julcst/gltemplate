#pragma once

#include "gl/buffer.hpp"

template <typename T>
class UniformBuffer {
   public:
    UniformBuffer(unsigned int index);
    UniformBuffer(unsigned int index, const T& uniforms);
    void upload(const T& uniforms);
    void bind(unsigned int index);

    Buffer<GL_UNIFORM_BUFFER> buffer;
};

template <typename T>
UniformBuffer<T>::UniformBuffer(unsigned int index) {
    buffer.bind(index);
    buffer.allocate(sizeof(T), GL_DYNAMIC_DRAW);
}

template <typename T>
UniformBuffer<T>::UniformBuffer(unsigned int index, const T& uniforms) {
    buffer.bind(index);
    buffer.load(uniforms, GL_DYNAMIC_DRAW);
}

template <typename T>
void UniformBuffer<T>::upload(const T& uniforms) {
    buffer.set(uniforms);
}

template <typename T>
void UniformBuffer<T>::bind(unsigned int index) {
    buffer.bind(index);
}