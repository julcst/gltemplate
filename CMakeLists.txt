cmake_minimum_required(VERSION 3.26)

project(GLTemplate VERSION 1.0 LANGUAGES C CXX)

# Load custom modules from the cmake directory
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Build framework
add_subdirectory(src/framework)

# Only build examples and zip sources if this is the top level project
if(PROJECT_IS_TOP_LEVEL)
    # Zip sources
    include(ZippingTool)
    # Full source zip, containing examples and resources
    #zip_sources(${PROJECT_NAME})
    # Minimal source zip, only containing the framework
    #zip_sources(${PROJECT_NAME}-minimal IGNORE icons/* meshes/* shaders/* textures/* src/demo/* src/minimalexample/*)

    # Build examples
    include(DeploymentTool)
    add_subdirectory(src/minimalexample)
    add_subdirectory(src/demo)
    setup_cpack()
endif()