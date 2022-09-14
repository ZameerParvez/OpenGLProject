cmake_minimum_required(VERSION 3.7...3.20)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    Main 
    VERSION 1.0
    DESCRIPTION "Demo Graphics Project"
    LANGUAGES CXX
)

set(DIR ".")

set(RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
configure_file(${DIR}/config.h.in ${DIR}/config.h)

# NOTE: When adding new demoes, a reference to it's cpp must be added here 
# for demos
set(DEMO_DIR ${DIR}/demos)
set(DEMOS_SRC
    ${DEMO_DIR}/demos.cpp
    ${DEMO_DIR}/simpleTriangle.cpp
    ${DEMO_DIR}/simpleRectangle.cpp
    ${DEMO_DIR}/simpleTexture.cpp
    ${DEMO_DIR}/simple3D.cpp
)
add_library(Demos STATIC ${DEMOS_SRC})
target_include_directories(Demos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
target_link_libraries(Demos PUBLIC Libs Systems)

# for entry point and top level sources
set(SOURCES ${DIR}/main.cpp)
add_executable(Main ${SOURCES})
target_link_libraries(Main PUBLIC Libs Demos Systems)
target_include_directories(Main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# prints debug stuff
# set(CMAKE_DEBUG_TARGET_PROPERTIES INCLUDE_DIRECTORIES)
