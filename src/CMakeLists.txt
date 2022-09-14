cmake_minimum_required(VERSION 3.7...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    Project VERSION 1.0
    DESCRIPTION "Graphics Tutorial Source"
    LANGUAGES CXX
)

add_compile_options(
    -std=c++20;-Wall;-Wno-error=uninitialized
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:DEBUG>:-ggdb3>
)

add_compile_definitions(
        $<$<CONFIG:RELEASE>:NDEBUG>
)

add_subdirectory("demo-project")
add_subdirectory("systems")

