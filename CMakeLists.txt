cmake_minimum_required(VERSION 3.10)
project(golOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# This sets an absolute path to the shaders directory
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders/")
set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets/")

# Define macro in compile flags
add_compile_definitions(SHADER_DIR=\"${SHADER_DIR}\")
add_compile_definitions(ASSET_DIR=\"${ASSET_DIR}\")

# Optional flags
option(BUILD_DEBUG "Build with debug flags" ON)
option(BUILD_CLEAN "Add custom clean target" ON)

# Include directories
include_directories(include)

# Source files
add_executable(OpenGLProject
    src/main.cpp
    src/Window.cpp
    src/GameOfLife.cpp
    src/Shader.cpp
    src/Renderer.cpp
)

# External dependencies
add_subdirectory(external/glad)
add_subdirectory(external/glfw)

# Link libraries
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(OpenGLProject PRIVATE
    glad
    glfw
    OpenGL::GL
    Threads::Threads
)

# Add custom clean target
if(BUILD_CLEAN)
    add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Build directory cleaned."
    )
endif()