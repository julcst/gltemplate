# Helps with deployment of the application

# Bundles resource files into the application bundle/directory and install the target at the right location
# Parameters:
#   TARGET: The target to bundle the resources with
#   RES_FILES: The list of resource files to bundle (absolute or relative to CMAKE_SOURCE_DIR)
function(bundle_resources TARGET)
    set(RES_FILES ${ARGN})
    set(MACOSX_ICON_FILE ${CMAKE_SOURCE_DIR}/icons/AppIcon.icns)
    cmake_path(GET MACOSX_ICON_FILE FILENAME MACOSX_ICON_FILE_NAME)

    # Add resource files to the target so they are included in the build
    if(CMAKE_VERSION LESS 3.23)
        # Mark all resources as headers so cmake does not try to compile them
        foreach(RES_FILE IN LISTS RES_FILES)
            set_source_files_properties(${RES_FILE} PROPERTIES HEADER_FILE_ONLY TRUE)
        endforeach()
        target_sources(${TARGET} PRIVATE ${RES_FILES} ${MACOSX_ICON_FILE})
    else()
        target_sources(${TARGET}
            PUBLIC
                FILE_SET resources TYPE HEADERS BASE_DIRS ${CMAKE_SOURCE_DIR} FILES ${RES_FILES}
                FILE_SET icon TYPE HEADERS BASE_DIRS ${CMAKE_SOURCE_DIR} FILES ${MACOSX_ICON_FILE}
        )
    endif()

    if(APPLE)
        # Bundling macOS application
        set_target_properties(${TARGET} PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE ${MACOSX_ICON_FILE_NAME}
        )

        # Copy resource files into bundle
        foreach(RES_FILE IN LISTS RES_FILES)
            cmake_path(GET RES_FILE PARENT_PATH LOC)
            cmake_path(RELATIVE_PATH LOC BASE_DIRECTORY ${CMAKE_SOURCE_DIR})
            set_source_files_properties(${RES_FILE} PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${LOC}"
            )
            message(DEBUG "Bundle ${RES_FILE} into macOS Application Bundle for target ${TARGET} at Resources/${LOC}")
        endforeach()
    
        set_source_files_properties(${MACOSX_ICON_FILE} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
        )
    
        # Packaging macOS installer
        install(TARGETS ${TARGET} BUNDLE DESTINATION .)
        
    elseif(WIN32)
        # Make the application run without a console window in Release mode
        set_target_properties(${TARGET} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
            LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
        )

        # Install resource files

        # foreach(RES_FILE IN LISTS ${RES_FILES})
        #     cmake_path(GET RES_FILE PARENT_PATH LOC)
        #     file(RELATIVE_PATH LOC_REL ${CMAKE_SOURCE_DIR} ${LOC})
        #     install(FILES ${RES_FILE} DESTINATION ${LOC})
        # endforeach()
    
        # Packaging Windows portable
        include(InstallRequiredSystemLibraries)
        install(TARGETS ${TARGET} DESTINATION .)
    endif()
endfunction()

# Setup CPack for packaging the application
function(setup_cpack)
    if(APPLE)
        set(CPACK_GENERATOR DragNDrop)
    elseif(WIN32)
        set(CPACK_GENERATOR ZIP) # For portable ZIP
        #set(CPACK_GENERATOR NSIS) # For .EXE installer, requires NSIS installed (winget install NSIS.NSIS)
        #set(CPACK_GENERATOR WIX) # For .MSI installer, requires WiX Toolset installed
    endif()
    #set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
    #set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)

    # Packaging source code (only works with Ninja and Makefiles)
    set(CPACK_SOURCE_GENERATOR ZIP)
    set(CPACK_SOURCE_IGNORE_FILES
        /.git/
        /build/
        /composed/
        /.*.DS_Store$
    )
    include(CPack)
endfunction()