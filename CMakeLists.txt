cmake_minimum_required(VERSION 3.26)

project(nebula VERSION 0.1.0)

# This moves the SDL dll to the right place (when BUILD_SHARED_LIBS is ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

# Uncomment to use static lib
# option(BUILD_SHARED_LIBS OFF)

add_subdirectory(libs/SDL3)

add_executable(nebula)
target_sources(nebula PRIVATE src/nebula.cpp)
target_link_libraries(nebula PRIVATE SDL3::SDL3)