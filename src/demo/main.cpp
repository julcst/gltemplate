#include <iostream>

#include "mainapp.hpp"

int main() {
    try {
        MainApp app;
        app.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}