cmake_minimum_required(VERSION 3.26)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)
    set(CMAKE_MACOSX_BUNDLE True)
elseif(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

project(GLTemplate VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

####################################################################
# Fetching libraries
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
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
# List files
set(SRC
    src/main.cpp
    src/example.cpp
    src/framework/app.cpp
    src/framework/camera.cpp
    src/framework/gl/buffer.cpp
    src/framework/gl/mesh.cpp
    src/framework/gl/program.cpp
    src/framework/gl/shader.cpp
    src/framework/gl/vertexarray.cpp
)
set(SHADERS
    shaders/example.frag
    shaders/projection.vert
    shaders/random.glsl
    shaders/screen.vert
    shaders/voxels.frag
)
set(ICON
    icons/AppIcon.icns
)

####################################################################
# Add executable
add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${SRC}
    ${SHADERS}
    ${ICON}
)

target_link_libraries(${PROJECT_NAME} glad glfw glm imgui_glfw)
configure_file("src/framework/config.hpp.in" "src/framework/config.hpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/src/framework)

####################################################################
if(APPLE)
    # Bundling macOS application
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER de.julcs.${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE AppIcon
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION}
    )
    set_source_files_properties(${ICON}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    set_source_files_properties(${SHADERS}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/shaders"
    )

    # Packaging macOS installer
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
    include(InstallRequiredSystemLibraries)
    set(CPACK_GENERATOR DragNDrop)
    set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
    include(CPack)
elseif(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
    install(FILES ${SHADERS} DESTINATION shaders)
    include(InstallRequiredSystemLibraries)
    set(CPACK_GENERATOR ZIP)
    set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
    include(CPack)
endif()