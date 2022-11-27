project(graphs)

set(_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Target: libgraphs
add_library(${PROJECT_NAME} SHARED
    "${_INCLUDE}/graphs/adjacency_matrix.hpp"
    "${_SRC}/adjacency_matrix.cpp"
)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${_INCLUDE}
)

# 3rd-party libraries
find_package(fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
