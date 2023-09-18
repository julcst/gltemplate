#include "example.hpp"

#include <iostream>

int main() {
    ExampleApp app;
    try {
        app.run();
    } catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}