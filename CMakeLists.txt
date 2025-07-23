cmake_minimum_required(VERSION 3.26)

project(nebula VERSION 0.1.0)

# This moves the SDL dll to the right place (when BUILD_SHARED_LIBS is ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

add_library(glad STATIC 
    libs/glad/glad.c
    libs/glad/include/glad/glad.h
    libs/glad/include/KHR/khrplatform.h
)

add_library(nebula_window STATIC
    src/modules/window/Window.cpp
    src/modules/window/Window.h
)
target_link_libraries(nebula_window PUBLIC 
    SDL3::SDL3
)

add_library(nebula_graphics STATIC
#    src/modules/graphics/Graphics.cpp
#    src/modules/graphics/Graphics.h
    src/modules/graphics/Shader.cpp
    src/modules/graphics/Shader.h
)
target_link_libraries(nebula_graphics PUBLIC 
    SDL3::SDL3
    glad
)

set(NEBULA_MODULES
    nebula_window
    nebula_graphics
)

set(NEBULA_LIBRARIES
    SDL3::SDL3
    glad
)

include_directories(
    BEFORE
    src
    libs
    libs/glad/include
    src/modules
)

# Uncomment to use static lib
# option(BUILD_SHARED_LIBS OFF)
add_subdirectory(libs/SDL3)

add_executable(nebula)
target_sources(nebula PRIVATE src/nebula.cpp)
target_link_libraries(nebula ${NEBULA_MODULES} ${NEBULA_LIBRARIES})
#target_include_directories(nebula PRIVATE libs/glad/include/)