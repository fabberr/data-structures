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
