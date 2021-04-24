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
set(SOURCES
    "${SRCDIR}/main.cpp"
)

add_executable(Main ${SOURCES})

target_link_libraries(Main PUBLIC Libs)

target_include_directories(
    Main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)