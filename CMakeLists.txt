cmake_minimum_required(VERSION 3.26)
project(gltemplate VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

####################################################################
# Fetching libraries
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.9
)
FetchContent_MakeAvailable(imgui)
add_library(imgui_glfw STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_glfw PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui_glfw glfw)

####################################################################
# Configure header file
configure_file("src/framework/config.hpp.in" "src/framework/config.hpp")
target_include_directories(imgui_glfw PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/src/framework)

####################################################################
# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/example.cpp
    src/framework/app.cpp
    src/framework/gl/buffer.cpp
    src/framework/gl/mesh.cpp
    src/framework/gl/program.cpp
    src/framework/gl/shader.cpp
    src/framework/gl/vertexarray.cpp
)

target_link_libraries(${PROJECT_NAME} glad glfw glm imgui_glfw)

####################################################################
# Building installer
set(CPACK_BUNDLE_NAME "gltemplate")
include(CPack)