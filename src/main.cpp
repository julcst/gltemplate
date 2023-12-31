#include <iostream>

#include "example.hpp"

int main() {
    try {
        ExampleApp app;
        app.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}