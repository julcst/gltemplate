#pragma once

#include <array>

/**
 * Implements a circular buffer that holds a fixed number of measurements and continously computes the average and the sum.
 * Makes (in my opinion) very cool usage of the wonderful concept of circular buffers to keep the time complexity O(1) for all operations
 */
template <typename T, size_t N>
class Series {
   public:
    T sum = 0;
    T avg = 0;

    void push(T measurement) {
        head = (head + 1) % N; // increment head and wrap around if necessary
        // head now points to the oldest measurement
        if (count < N) {
            count++;
        } else {
            sum -= measurements[head]; // remove oldest measurement from sum
        }
        measurements[head] = measurement; // override oldest measurement with new one
        sum += measurement; // add new measurement to sum
        avg = sum / count;
    }

    T newest() const {
        return measurements[head];
    }

   private:
    std::array<T, N> measurements;
    size_t head = 0;
    size_t count = 0;
};