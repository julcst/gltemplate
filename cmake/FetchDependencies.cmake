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

# Known issues: Generates warning with CMake >= 3.20, see policy CMP0120
# glbinding
FetchContent_Declare(
    glbinding
    #GIT_REPOSITORY https://github.com/cginternals/glbinding.git
    GIT_TAG v3.3.0
    URL https://github.com/cginternals/glbinding/archive/v3.3.0.tar.gz
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS # First try to find the package in the system, if not found download it locally. For example use `brew install glbinding` on macOS
)
set(OPTION_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(OPTION_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# GLM
FetchContent_Declare(
    glm
    #GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    URL https://github.com/g-truc/glm/archive/1.0.1.tar.gz
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS # First try to find the package in the system, if not found download it locally. For example use `brew install glm` on macOS
)
add_compile_definitions(GLM_FORCE_RADIANS)

# ImGui
FetchContent_Declare(
    imgui
    #GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.8
    URL https://github.com/ocornut/imgui/archive/v1.90.8.tar.gz
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

FetchContent_MakeAvailable(glbinding glfw3 glm imgui tinyobjloader)

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
file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h ${stb_SOURCE_DIR}/stb_image_write.h)
# Known issues: stb_image_write.h uses sprintf_s which is only available on Windows, on other platforms it uses sprintf throws deprecation warnings, see https://github.com/nothings/stb/issues/1446
# Workaround:
file(GENERATE
    OUTPUT ${stb_SOURCE_DIR}/stb.cpp
    CONTENT
"#ifndef __STDC_LIB_EXT1__
#define __STDC_LIB_EXT1__
#define sprintf_s snprintf
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>"
)
add_library(stb STATIC ${stb_SOURCE_DIR}/stb.cpp)
target_include_directories(stb PUBLIC ${stb_SOURCE_DIR})