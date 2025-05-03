cmake_minimum_required(VERSION 3.12)
project(engine)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug/Release)" FORCE)
endif()

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE SOURCES ${SOURCES} glad/glad.c)

# Add executable
add_executable(engine ${SOURCES})

# Add compile options based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # -g flag for debugging symbols
    target_compile_options(engine PRIVATE -g -DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Optimization flag for release builds. You can change -O3 to other optimization levels if desired.
    target_compile_options(engine PRIVATE -O3)
endif()

# Include directories (assumes headers are in PATH-visible locations)
include_directories($ENV{VULKAN_SDK}/Include)
include_directories($ENV{GLFW_INCLUDE_DIR})  # Optional, if GLFW headers are not in a standard include path
include_directories(src)
include_directories(glad)

# Link libraries (assuming they are in the system linker path)
target_link_libraries(engine PRIVATE glfw vulkan)

# Output some informational messages
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
