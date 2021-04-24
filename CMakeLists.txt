cmake_minimum_required(VERSION 3.7...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Project VERSION 1.0
                  DESCRIPTION "Graphics tutorial source"
                  LANGUAGES CXX)

add_subdirectory("lib")
add_subdirectory("Main")