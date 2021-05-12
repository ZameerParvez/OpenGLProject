cmake_minimum_required(VERSION 3.7...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    Main 
    VERSION 1.0
    DESCRIPTION "Graphics tutorial source"
    LANGUAGES CXX
)

add_compile_options(
       -std=c++17 -Wall -Wno-error=maybe-uninitialized
       $<$<CONFIG:RELEASE>:-Ofast>
       $<$<CONFIG:DEBUG>:-O0>
       $<$<CONFIG:DEBUG>:-ggdb3>
)
add_compile_definitions(
        $<$<CONFIG:RELEASE>:NDEBUG>
)

set(SRCDIR "src")

# add the renderer library
add_subdirectory("${SRCDIR}/renderer")

# copy resources to build directory
# TODO: don't do this, instead at runtime use a full path to a resources folder, and all of the resources like shaders and textures will be stored in there, and referenced in code with a relative path
file(COPY "${SRCDIR}/resources" DESTINATION "${SRCDIR}")


# NOTE: When adding new demoes, a reference to it's cpp must be added here 
# for demos
set(DEMO_DIR "${SRCDIR}/demos")
set(DEMOS_SRC
    "${DEMO_DIR}/demos.cpp"
    "${DEMO_DIR}/simpleTriangle.cpp"
    "${DEMO_DIR}/simpleRectangle.cpp"
    "${DEMO_DIR}/simpleTexture.cpp"
)
add_library(Demos STATIC ${DEMOS_SRC})
target_link_libraries(Demos PUBLIC Libs Renderer)
target_include_directories(
    Demos
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SRCDIR}/renderer
)

# for entry point and top level sources
set(SOURCES
    "${SRCDIR}/main.cpp"
)
add_executable(Main ${SOURCES})
target_link_libraries(Main PUBLIC Libs Demos Renderer)
target_include_directories(
    Main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
