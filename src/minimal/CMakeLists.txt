set(SRC
    main.cpp
    mainapp.cpp
)
set(HEADERS
    mainapp.hpp
)

add_executable(minimal ${SRC} ${HEADERS})
target_include_directories(minimal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(minimal framework)
BundleResources(minimal)