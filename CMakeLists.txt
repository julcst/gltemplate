cmake_minimum_required(VERSION 3.26)

project(GLTemplate VERSION 1.0 LANGUAGES CXX)
set(PROJECT_IDENTIFIER com.example)
set(ARCHIVE_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.zip)

# Source files
set(SRC
    src/main.cpp
    src/mainapp.cpp
    src/framework/app.cpp
    src/framework/camera.cpp
    src/framework/common.cpp
    src/framework/imguiutil.cpp
    src/framework/mesh.cpp
    src/framework/objparser.cpp
    src/framework/series.hpp
    src/framework/gl/buffer.cpp
    src/framework/gl/program.cpp
    src/framework/gl/query.cpp
    src/framework/gl/shader.cpp
    src/framework/gl/texture.cpp
    src/framework/gl/vertexarray.cpp
)
set(INCLUDE
    src/
)

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

set(RES "${SHADERS};${MESHES};${TEXTURES}")

# Mark resorce files as header only to avoid confusion with source files
set_source_files_properties(${RES} PROPERTIES HEADER_FILE_ONLY True)

set(ICON
    icons/AppIcon.icns
)

######################### Configure build #########################
# Make the application run without a console window in Release mode   
if(WIN32 AND $<CONFIG:Release>)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()
# Enable universal building and bundling on macOS
set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)
set(CMAKE_MACOSX_BUNDLE True)
# Disable warnings
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(${PROJECT_NAME}
    ${SRC}
    ${RES}
    ${ICON}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})

# Link dependencies
include(Dependencies.cmake)
target_link_libraries(${PROJECT_NAME} glad glfw glm imgui_glfw tinyobjloader stb_impl nlohmann_json::nlohmann_json)

# Configure config.hpp
configure_file("src/config.hpp.in" "src/config.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src/)

######################## CPack deployment #########################
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

######################### Zipping sources #########################
file(GLOB_RECURSE SRC_FILES *)
# Ignore build files
list(FILTER SRC_FILES EXCLUDE REGEX ".*.DS_Store$|${BUILD_DIR}//*|.git//*|composed//*|.vscode//*|.idea//*")

add_custom_target(zip ALL
    COMMAND ${CMAKE_COMMAND} -E tar c "${ARCHIVE_FILE}" --format=zip -- ${SRC_FILES}
    COMMENT "Zipping sources"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)
