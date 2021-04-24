cmake_minimum_required(VERSION 3.7...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    Libs 
    VERSION 1.0 
    DESCRIPTION "Collection of useful libraries for graphics programming"
)

# GLAD
set(GLAD_DIR "glad")
set(GLAD_SRC "${GLAD_DIR}/src")
set(GLAD_INC "${GLAD_DIR}/include")

add_library(glad STATIC "${GLAD_SRC}/glad.c")
target_include_directories(glad PUBLIC "${GLAD_INC}")

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only")

add_subdirectory(glfw)

# IMGUI
set(IMGUI_DIR "imgui")
set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
set(IMGUI_HEAD
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
    ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
)

add_library(
    imgui STATIC
    "${IMGUI_SRC}"
    "${IMGUI_HEAD}"
)

target_include_directories( 
    imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui glad glfw)


# OpenGL
# I think that this is mainly for linux?
find_package(OpenGL REQUIRED)

# Final Links
add_library(Libs INTERFACE)
target_link_libraries(Libs INTERFACE glad OpenGL::GL glfw imgui)