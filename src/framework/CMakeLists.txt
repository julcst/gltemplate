set(SRC
    app.cpp
    camera.cpp
    common.cpp
    imguiutil.cpp
    mesh.cpp
    objparser.cpp
    gl/buffer.cpp
    gl/program.cpp
    gl/query.cpp
    gl/shader.cpp
    gl/texture.cpp
    gl/vertexarray.cpp
)

set(HEADERS
    app.hpp
    camera.hpp
    common.hpp
    context.hpp
    imguiutil.hpp
    mesh.hpp
    objparser.hpp
    series.hpp
    uniformbuffer.hpp
    gl/buffer.hpp
    gl/program.hpp
    gl/query.hpp
    gl/shader.hpp
    gl/texture.hpp
    gl/vertexarray.hpp
)

configure_file(context.hpp.in context.hpp)

add_library(framework STATIC ${SRC} ${HEADERS})
target_compile_features(framework PUBLIC cxx_std_17) # Require C++17 (filesystem, initializer list, constexpr, ...)
target_include_directories(framework
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

include(FetchDependencies)
target_link_libraries(framework
    PUBLIC
        glad
        glm
        glfw
        imgui_glfw
        stb_impl
        tinyobjloader
)