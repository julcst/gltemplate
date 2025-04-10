# This code fetches all the required dependencies from their respective repositories using the FetchContent module
include(FetchContent)

# Disbale warnings for third party libraries
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)

# Known issues: Frametimes are weird on macOS with glfw > 3.3 and VSync enabled but with glfw < 3.4 deprecation warnings are thrown
# GLFW
FetchContent_Declare(
    glfw3
    #GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS # First try to find the package in the system, if not found download it locally. For example use `brew install glfw` on macOS
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# # GLAD2
# FetchContent_Declare(
#     glad2
#     #GIT_REPOSITORY https://github.com/Dav1dde/glad.git
#     GIT_TAG v2.0.8
#     URL https://github.com/Dav1dde/glad/archive/v2.0.8.tar.gz
#     EXCLUDE_FROM_ALL
# )

# GLAD
FetchContent_Declare(
    glad
    #GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    URL https://github.com/Dav1dde/glad/archive/v0.1.36.tar.gz
    EXCLUDE_FROM_ALL
)
set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c-debug" CACHE STRING "" FORCE)

# GLM
FetchContent_Declare(
    glm
    #GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    URL https://github.com/g-truc/glm/archive/1.0.1.tar.gz
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS # First try to find the package in the system, if not found download it locally. For example use `brew install glm` on macOS
)
add_compile_definitions(GLM_FORCE_RADIANS)

# ImGui
FetchContent_Declare(
    imgui
    #GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    URL https://github.com/ocornut/imgui/archive/v1.91.9b.tar.gz
    EXCLUDE_FROM_ALL
)

# tinyobjloader
FetchContent_Declare(
    tinyobjloader
    #GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v2.0.0rc13
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    URL https://github.com/tinyobjloader/tinyobjloader/archive/v2.0.0rc13.tar.gz
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(glad glfw3 glm imgui tinyobjloader)

# add_subdirectory(${glad_SOURCE_DIR}/cmake ${glad_BINARY_DIR})
# glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

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
target_link_libraries(imgui_glfw PUBLIC glfw)
target_compile_features(imgui_glfw PUBLIC cxx_std_17)

# stb is a header only library without CMake so we need to add it manually
set(stb_SOURCE_DIR ${CMAKE_BINARY_DIR}/external/stb)
file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h ${stb_SOURCE_DIR}/stb_image.h)
file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h  ${stb_SOURCE_DIR}/stb_image_write.h)
# Known issues: stb_image_write.h uses sprintf_s which is only available on Windows, on other platforms it uses sprintf throws deprecation warnings, see https://github.com/nothings/stb/issues/1446
# Workaround: Replace sprintf with snprintf
file(READ ${stb_SOURCE_DIR}/stb_image_write.h stb_image_write_content)
string(REPLACE "sprintf(buffer," "snprintf(buffer, sizeof(buffer)," stb_image_write_content "${stb_image_write_content}")
file(WRITE ${stb_SOURCE_DIR}/stb_image_write.h "${stb_image_write_content}")
file(GENERATE
    OUTPUT ${stb_SOURCE_DIR}/stb.cpp
    CONTENT
"#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>"
)
add_library(stb STATIC ${stb_SOURCE_DIR}/stb.cpp)
target_include_directories(stb PUBLIC ${stb_SOURCE_DIR})