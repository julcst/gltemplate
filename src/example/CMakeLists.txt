set(SRC
    main.cpp
    mainapp.cpp
)
set(HEADERS
    mainapp.hpp
)

add_executable(example ${SRC} ${HEADERS})
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(example framework)
BundleResources(example)