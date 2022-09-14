# cmake_minimum_required(VERSION 3.7...3.20)

# if(${CMAKE_VERSION} VERSION_LESS 3.12)
#     cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
# endif()

# project(
#     Renderer 
#     VERSION 1.0
#     DESCRIPTION "Renderer"
#     LANGUAGES CXX
# )

# set(SOURCES
# "graphicsLibs.cpp"
# "shader.cpp"
# "texture.cpp"
# "camera.cpp"
# )
# add_library(Renderer STATIC ${SOURCES})
# # IDEA: maybe only link with the things needed, which would just be glad
# target_link_libraries(Renderer PUBLIC Libs)