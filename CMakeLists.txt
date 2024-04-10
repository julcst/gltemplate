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
if(WIN32)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WARN_DEPRECATED FALSE)

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
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD
# FetchContent_Declare(
#     glad
#     GIT_REPOSITORY https://github.com/Dav1dde/glad.git
#     GIT_TAG v0.1.36
# )
FetchContent_Declare(
    glad
    URL https://github.com/Dav1dde/glad/archive/refs/tags/v0.1.36.tar.gz
)
FetchContent_MakeAvailable(glad)

# GLM
# FetchContent_Declare(
#     glm
#     GIT_REPOSITORY https://github.com/g-truc/glm.git
#     GIT_TAG 0.9.9.8
# )
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.tar.gz
)
FetchContent_MakeAvailable(glm)

# ImGui
# FetchContent_Declare(
#     imgui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git
#     GIT_TAG v1.89.9
# )
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.4.tar.gz
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

# objgl
FetchContent_Declare(
    objgl
    GIT_REPOSITORY https://github.com/karolek471/objgl.git
    GIT_TAG a038f0eb432f2167ae186f7c74cb4c07eb5df343
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
FetchContent_MakeAvailable(objgl)
add_library(objgl STATIC
    ${objgl_SOURCE_DIR}/source/objgl.c
)
target_include_directories(objgl PUBLIC ${objgl_SOURCE_DIR}/source)

# json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# stb_image
FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)
FetchContent_MakeAvailable(stb_image)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})


####################################################################
# Source files
set(SRC
    src/main.cpp
    src/example.cpp
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
    shaders/voxels.frag
)
set(MODELS
    models/bunny.obj
    models/cube.obj
    models/cylinder.obj
    models/donut.obj
    models/highpolysphere.obj
    models/plane.obj
    models/suzanne.obj
)
set(CUBEMAPS
)
foreach(cubemap ${CUBEMAPS})
    list(APPEND CUBEMAP_FILES ${cubemap}_nx.hdr ${cubemap}_ny.hdr ${cubemap}_nz.hdr ${cubemap}_px.hdr ${cubemap}_py.hdr ${cubemap}_pz.hdr)
endforeach()
set(ICON
    icons/AppIcon.icns
)
set(RES "${SHADERS};${MODELS};${MATERIALS};${CUBEMAP_FILES}")
set_source_files_properties(${RES} PROPERTIES HEADER_FILE_ONLY True)

####################################################################
# Add executable
add_executable(${PROJECT_NAME}
    ${SRC}
    ${RES}
    ${ICON}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})

target_link_libraries(${PROJECT_NAME} glad glfw glm imgui_glfw objgl nlohmann_json::nlohmann_json stb_image)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src/)

foreach(model ${MODELS})
    set(MODEL_FILES "${MODEL_FILES}\"${model}\",")
endforeach()
foreach(material ${MATERIALS})
    set(MATERIAL_FILES "${MATERIAL_FILES}\"${material}\",")
endforeach()
foreach(cubemap ${CUBEMAPS})
    set(CUBEMAP_NAMES "${CUBEMAP_NAMES}\"${cubemap}\",")
endforeach()
configure_file("src/config.hpp.in" "src/config.hpp")

####################################################################
if(APPLE)
    # Bundling macOS application
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.julcs.${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE AppIcon
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION}
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
    # include(InstallRequiredSystemLibraries)
    set(CPACK_GENERATOR DragNDrop)
    set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
    include(CPack)
    
elseif(WIN32)
    foreach(RES_FILE IN LISTS RES)
        cmake_path(GET RES_FILE PARENT_PATH LOC)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${RES_FILE} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${RES_FILE}
        )
        install(FILES ${RES_FILE} DESTINATION ${LOC})
    endforeach()

    # Packaging Windows portable
    include(InstallRequiredSystemLibraries)
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
    set(CPACK_GENERATOR ZIP) # For portable ZIP
    #set(CPACK_GENERATOR NSIS) # For .EXE installer, requires NSIS installed (winget install NSIS.NSIS)
    #set(CPACK_GENERATOR WIX) # For .MSI installer, requires WiX Toolset installed
    set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
    set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
    include(CPack)
endif()