cmake_minimum_required(VERSION 3.26)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

if(APPLE)
    # Enable universal building on macOS
    set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)
    set(CMAKE_MACOSX_BUNDLE True)
elseif(WIN32)
    # Make the application run without a console window in Release mode
    if($<CONFIG:Release>)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endif()

project(GLTemplate VERSION 1.0 LANGUAGES CXX C)
set(PROJECT_IDENTIFIER com.julcs)
if(WIN32)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)

####################################################################
# Fetching libraries
include(FetchContent)

# GLFW
# FetchContent_Declare(
#     glfw
#     GIT_REPOSITORY https://github.com/glfw/glfw.git
#     GIT_TAG 3.3
# )
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.tar.gz
    EXCLUDE_FROM_ALL
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
    glad
    #GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
    URL https://github.com/Dav1dde/glad/archive/v0.1.36.tar.gz
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glad)

# GLM
FetchContent_Declare(
    glm
    #GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    URL https://github.com/g-truc/glm/archive/1.0.1.tar.gz
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glm)

# ImGui
FetchContent_Declare(
    imgui
    #GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4
    URL https://github.com/ocornut/imgui/archive/v1.90.4.tar.gz
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)
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
target_include_directories(imgui_glfw PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui_glfw glfw)

# tinyobjloader
FetchContent_Declare(
    tinyobjloader
    #GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v2.0.0rc13
    URL https://github.com/tinyobjloader/tinyobjloader/archive/v2.0.0rc13.tar.gz
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(tinyobjloader)

# json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(json)

# stb_image
# FetchContent_Declare(
#     stb
#     GIT_REPOSITORY https://github.com/gracicot/stb-cmake.git
#     GIT_TAG d42aa7a48ff0479fa9cab999d035a901cc4c4134
#     EXCLUDE_FROM_ALL
# )
# FetchContent_MakeAvailable(stb)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(stb)
add_library(stb INTERFACE)
target_compile_definitions(stb INTERFACE STB_IMAGE_IMPLEMENTATION)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

####################################################################
# Source files
set(SRC
    src/main.cpp
    src/mainapp.cpp
    src/framework/app.cpp
    src/framework/camera.cpp
    src/framework/common.cpp
    src/framework/series.hpp
    src/framework/util.cpp
    src/framework/gl/buffer.cpp
    src/framework/gl/mesh.cpp
    src/framework/gl/program.cpp
    src/framework/gl/query.cpp
    src/framework/gl/shader.cpp
    src/framework/gl/texture.cpp
    src/framework/gl/vertexarray.cpp
)
set(INCLUDE
    src/
    src/framework/
)

####################################################################
# Resource files
set(SHADERS
    shaders/background.frag
    shaders/debug.glsl
    shaders/projection.vert
    shaders/random.glsl
    shaders/screen.vert
    shaders/tangentspace.glsl
    shaders/uniforms.glsl
    shaders/debug.frag
)
set(MESHES
    meshes/bunny.obj
    meshes/cube.obj
    meshes/cylinder.obj
    meshes/donut.obj
    meshes/highpolysphere.obj
    meshes/plane.obj
    meshes/suzanne.obj
)
set(TEXTURES
    textures/test.png
)
set(ICON
    icons/AppIcon.icns
)
set(RES "${SHADERS};${MESHES};${TEXTURES}")
set_source_files_properties(${RES} PROPERTIES HEADER_FILE_ONLY True)
foreach(mesh ${MESHES})
    set(MESH_FILES "${MESH_FILES}\"${mesh}\",")
endforeach()

####################################################################
# Add executable
add_executable(${PROJECT_NAME}
    ${SRC}
    ${RES}
    ${ICON}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})

target_link_libraries(${PROJECT_NAME} glad glfw glm imgui_glfw tinyobjloader nlohmann_json::nlohmann_json stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src/)

configure_file("src/config.hpp.in" "src/config.hpp")

####################################################################
# CPack deployment
if(APPLE)
    # Bundling macOS application
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_IDENTIFIER}.${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE AppIcon
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )

    # Copy resource files into bundle
    foreach(RES_FILE IN LISTS RES)
        cmake_path(GET RES_FILE PARENT_PATH LOC)
        set_source_files_properties(${RES_FILE}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/${LOC}"
        )
    endforeach()

    set_source_files_properties(${ICON}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    # Packaging macOS installer
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
    set(CPACK_GENERATOR DragNDrop)
    
elseif(WIN32)
    foreach(RES_FILE IN LISTS RES)
        cmake_path(GET RES_FILE PARENT_PATH LOC)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${RES_FILE} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${RES_FILE}
            COMMENT "Copying ${CMAKE_SOURCE_DIR}/${RES_FILE} to $<TARGET_FILE_DIR:${PROJECT_NAME}>/${RES_FILE}"
            VERBATIM
        )
        install(FILES ${RES_FILE} DESTINATION ${LOC})
    endforeach()

    # Packaging Windows portable
    include(InstallRequiredSystemLibraries)
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
    set(CPACK_GENERATOR ZIP) # For portable ZIP
    #set(CPACK_GENERATOR NSIS) # For .EXE installer, requires NSIS installed (winget install NSIS.NSIS)
    #set(CPACK_GENERATOR WIX) # For .MSI installer, requires WiX Toolset installed
endif()
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
include(CPack)

####################################################################
# Zipping sources
set(ARCHIVE_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}-source.zip)
file(GLOB_RECURSE SRC_FILES *)
# Ignore build files
list(FILTER SRC_FILES EXCLUDE REGEX ".*.DS_Store|build/*|.git/*|composed/*|.vscode/*")

add_custom_target(zip ALL
    COMMAND ${CMAKE_COMMAND} -E tar c "${ARCHIVE_FILE}" --format=zip -- ${SRC_FILES}
    COMMENT "Zipping sources"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)
