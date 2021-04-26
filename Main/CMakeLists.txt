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


set(SRCDIR "src")

# for demos
set(DEMO_DIR "${SRCDIR}/demos")
set(DEMOS_SRC
    "${DEMO_DIR}/demos.cpp"
    "${DEMO_DIR}/simpleTriangle.cpp"
    "${DEMO_DIR}/simpleRectangle.cpp"
)
add_library(Demos STATIC ${DEMOS_SRC})
target_link_libraries(Demos PUBLIC Libs)
target_include_directories(
    Demos
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# for entry point and top level sources
set(SOURCES
    "${SRCDIR}/main.cpp"
)
add_executable(Main ${SOURCES})
target_link_libraries(Main PUBLIC Libs Demos)
target_include_directories(
    Main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)