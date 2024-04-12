#pragma once

#include "buffer.hpp"

template <typename T>
class UniformBuffer {
   public:
    UniformBuffer(unsigned int index, const T& uniforms = T{});
    void upload(const T& uniforms);
    void bind(unsigned int index);

   private:
    Buffer buffer;
};

template <typename T>
UniformBuffer<T>::UniformBuffer(unsigned int index, const T& uniforms) : buffer() {
    buffer.bind(Buffer::Type::UNIFORM_BUFFER, index);
    buffer.load(Buffer::Type::UNIFORM_BUFFER, uniforms);
}

template <typename T>
void UniformBuffer<T>::upload(const T& uniforms) {
    buffer.set(Buffer::Type::UNIFORM_BUFFER, uniforms);
}

template <typename T>
void UniformBuffer<T>::bind(unsigned int index) {
    buffer.bind(Buffer::Type::UNIFORM_BUFFER, index);
}