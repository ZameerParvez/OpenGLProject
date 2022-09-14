cmake_minimum_required(VERSION 3.7...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    Systems 
    VERSION 1.0
    DESCRIPTION "Systems"
    LANGUAGES CXX
)

set(DIR systems)

set(RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/default-resources)
configure_file(${DIR}/config.h.in ${DIR}/config.h)

set(RENDERER_DIR ${DIR}/renderer)
set(RENDERER_SOURCES
    ${RENDERER_DIR}/graphicsLibs.cpp
    ${RENDERER_DIR}/shader.cpp
    ${RENDERER_DIR}/texture.cpp
    ${RENDERER_DIR}/camera.cpp
)
add_library(Renderer STATIC ${RENDERER_SOURCES})
target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
target_link_libraries(Renderer PUBLIC Libs)

add_library(Systems INTERFACE)
target_link_libraries(Systems INTERFACE Renderer)
