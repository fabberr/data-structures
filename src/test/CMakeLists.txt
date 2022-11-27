project(tests)

set(_INCLUDE_GRAPHS "${CMAKE_SOURCE_DIR}/graph/include")
set(_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# tests
add_executable(${PROJECT_NAME}
    "${_SRC}/main.cpp"
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE graphs
)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${_INCLUDE_GRAPHS}
)

# viz
add_custom_target(viz
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/graph.svg"
    VERBATIM
)
add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/graph.svg"
    COMMAND
        ${PROJECT_NAME} | dot -Tsvg > "${CMAKE_CURRENT_BINARY_DIR}/graph.svg"
    COMMAND
        mkdir "${CMAKE_CURRENT_SOURCE_DIR}/output/"
    COMMAND
        cp "${CMAKE_CURRENT_BINARY_DIR}/graph.svg" "${CMAKE_CURRENT_SOURCE_DIR}/output/graph.svg"
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${PROJECT_NAME}
    COMMENT
        "Generating graph vizualization"
    VERBATIM
)
