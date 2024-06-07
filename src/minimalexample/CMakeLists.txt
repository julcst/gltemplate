set(SRC
    main.cpp
    mainapp.cpp
)
set(HEADERS
    mainapp.hpp
)

add_executable(minimalexample ${SRC} ${HEADERS})
target_include_directories(minimalexample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(minimalexample framework)
bundle_resources(minimalexample)