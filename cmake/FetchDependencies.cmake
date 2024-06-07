# This code fetches all the required dependencies from their respective repositories using the FetchContent module
include(FetchContent)

# Disbale warnings for third party libraries
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# GLFW
FetchContent_Declare(
    glfw
    #GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.tar.gz
    EXCLUDE_FROM_ALL
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# GLAD
FetchContent_Declare(
    glad
    #GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
    URL https://github.com/Dav1dde/glad/archive/v0.1.36.tar.gz
    EXCLUDE_FROM_ALL
)

# GLM
add_compile_definitions(GLM_FORCE_RADIANS)
FetchContent_Declare(
    glm
    #GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    URL https://github.com/g-truc/glm/archive/1.0.1.tar.gz
    EXCLUDE_FROM_ALL
)

# ImGui
FetchContent_Declare(
    imgui
    #GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4
    URL https://github.com/ocornut/imgui/archive/v1.90.4.tar.gz
    EXCLUDE_FROM_ALL
)

# tinyobjloader
FetchContent_Declare(
    tinyobjloader
    #GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v2.0.0rc13
    URL https://github.com/tinyobjloader/tinyobjloader/archive/v2.0.0rc13.tar.gz
    EXCLUDE_FROM_ALL
)

# stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(glfw glad glm imgui tinyobjloader stb)

# ImGui is not build by CMake, so we need to add it manually
add_library(imgui_glfw STATIC
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_glfw PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends ${imgui_SOURCE_DIR}/misc/cpp ${glfw_SOURCE_DIR}/include)
target_compile_features(imgui_glfw PUBLIC cxx_std_17)

# stb is a header only library, so we need to add it as an interface library
add_library(stb_impl INTERFACE)
target_compile_definitions(stb_impl INTERFACE STB_IMAGE_IMPLEMENTATION)
target_include_directories(stb_impl INTERFACE ${stb_SOURCE_DIR})