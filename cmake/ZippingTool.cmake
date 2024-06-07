cmake_path(RELATIVE_PATH CMAKE_BINARY_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE BUILD_DIR)
set(ALWAYS_IGNORE
    .vscode/*
    .idea/*
    .cache/*
    .git/*
    .*.DS_Store$
    ${BUILD_DIR}/*)
   # CACHE STRING "Files to ignore when zipping the source directory")

# Essentialy a rewrite of CPACK_SOURCE_GENERATOR but CPACK_SOURCE_GENERATOR only works with Ninja and Makefile generators
# This function creates a custom target that zips the source directory
# Parameters: 
#   ZIP_TARGET - Name of the zip file and target to create
#   ZIP_IGNORE - Files to ignore when zipping the source directory
function(zip_sources ZIP_TARGET)
    set(ZIP_FILE ${CMAKE_BINARY_DIR}/${ZIP_TARGET}.zip)
    cmake_parse_arguments(ZIP "" "" "IGNORE" ${ARGN})

    file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/*)
    # Ignore build directories and other irrelevant files
    list(APPEND ZIP_IGNORE ${ALWAYS_IGNORE})
    list(JOIN ZIP_IGNORE "|" IGNORE_REGEX)
    list(FILTER SRC_FILES EXCLUDE REGEX ${IGNORE_REGEX})

    add_custom_target(${ZIP_TARGET}-zip ALL
        COMMAND ${CMAKE_COMMAND} -E tar c ${ZIP_FILE} --format=zip -- ${SRC_FILES}
        COMMENT "Creating ${ZIP_FILE} from ${SRC_FILES} filtered with regex ${IGNORE_REGEX}"
        DEPENDS ${SRC_FILES}
        BYPRODUCTS ${ZIP_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
    )
endfunction()