cmake_minimum_required(VERSION 3.26)

project(GLTemplate VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Resource files
set(SHADERS
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
    textures/checker.png
    textures/checkerbw.png
)

# Mark resorce files as headers to avoid confusion with compilable source files
list(APPEND RES ${SHADERS} ${MESHES} ${TEXTURES})
list(TRANSFORM RES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

# Link dependencies
add_subdirectory(src/framework)

# Zip sources
include(AutoZipTool)

include(DeploymentTool)
add_subdirectory(src/example)
SetupCPack()