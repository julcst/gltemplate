# Essentialy a rewrite of CPACK_SOURCE_GENERATOR but CPACK_SOURCE_GENERATOR only works with Ninja and Makefile generators
# Parameter: ARCHIVE_FILE - filepath to the generated archive file

set(ARCHIVE_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.zip CACHE FILEPATH "Path to the generated archive file")

file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} *)
set(BUILD_DIR ${CMAKE_BINARY_DIR})
cmake_path(RELATIVE_PATH BUILD_DIR)

# Ignore build directories and other irrelevant files
list(FILTER SRC_FILES EXCLUDE REGEX .*.DS_Store$|${BUILD_DIR}/*|.git/*|composed/*|.vscode/*|.idea/*)

add_custom_target(zip ALL
    COMMAND ${CMAKE_COMMAND} -E tar c "${ARCHIVE_FILE}" --format=zip -- ${SRC_FILES}
    COMMENT "Zipping sources: ${SRC_FILES}"
    DEPENDS ${SRC_FILES}
    BYPRODUCTS ${ARCHIVE_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)