set(SRC
    main.cpp
    mainapp.cpp
)
set(HEADERS
    mainapp.hpp
)

# Resource files
set(SHADERS
    shaders/background.frag
    shaders/debug.frag
    shaders/debug.glsl
    shaders/projection.vert
    shaders/random.glsl
    shaders/raygen.vert
    shaders/tangentspace.glsl
    shaders/uniforms.glsl
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

list(APPEND RES ${SHADERS} ${MESHES} ${TEXTURES})
list(TRANSFORM RES PREPEND ${CMAKE_SOURCE_DIR}/)

add_executable(demo ${SRC} ${HEADERS})
target_include_directories(demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(demo framework)
bundle_resources(demo ${RES})