# OpenGL Template

A simple wrapper framework around common OpenGL concepts I created to learn CMake, C++ and graphics programming and make it easier for me to develop multiplatform OpenGL applications.

## Features

- Automatically downloads dependencies with CMake `FetchContent`
- DMG generation with CPack
- Encapsulates OpenGL resources with RAII wrappers

## Dependencies
- [GLFW](https://github.com/glfw/glfw) for window management
- [Glad](https://github.com/Dav1dde/glad) as a loader for OpenGL
- [GLM](https://github.com/g-truc/glm) for vector math
- [Dear](https://github.com/ocornut/imgui) ImGui for GUI

## Note

This is my first project with CMake and C++, so some of my design choices may be suboptimal or inelegant and I welcome all kinds of suggestions and feedback.