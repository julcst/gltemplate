cmake_minimum_required(VERSION 3.22...3.31)

project(GLTemplate VERSION 1.0 LANGUAGES C CXX)

# Load custom modules from the cmake directory
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Build framework
add_subdirectory(src/framework)

# Only build examples and zip sources if this is the top level project
if(PROJECT_IS_TOP_LEVEL)
    # Build examples
    include(DeploymentTool)
    add_subdirectory(src/minimalexample)
    add_subdirectory(src/demo)
    setup_cpack()
endif()