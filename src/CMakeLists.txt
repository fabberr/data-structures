#
# Top-level CMakeLists.txt
#
cmake_minimum_required(VERSION 3.25.0)

# Prevents in-source builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Project setup
project(data-structs VERSION 1.0.0 LANGUAGES CXX)

set_directory_properties(PROPERTIES
    CXX_STANDARD 20
)

add_compile_options(
    -Wall -Wextra -Wpedantic -Wconversion -Wshadow
    $<$<CONFIG:RELEASE>:-O3>
    
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
)

add_compile_definitions(
    $<$<CONFIG:RELEASE>:NDEBUG>
    $<$<CONFIG:RELEASE>:_NDEBUG>
    
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:DEBUG>:_DEBUG>
)

# Add targets
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/graph")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
