cmake_minimum_required(VERSION 3.26)

project(GLTemplate VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Link dependencies
add_subdirectory(src/framework)

# Zip sources
include(AutoZipTool)

include(DeploymentTool)
add_subdirectory(src/minimal)
add_subdirectory(src/demo)
SetupCPack()