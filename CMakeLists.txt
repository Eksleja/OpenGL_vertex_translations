cmake_minimum_required(VERSION 3.10)
project(hello_window)

add_executable(hello_window
    src/main.cpp
    src/config.h
    src/glad.c   # <-- Make sure this is here
    src/triangle_mesh.cpp
    src/linAlgebra.cpp
)

# Include path (for glfw3.h)
target_include_directories(hello_window PRIVATE
    dependencies/include
)

# Library path (for libglfw3.a)
link_directories(${CMAKE_SOURCE_DIR}/dependencies/GLFW)

# Link libraries
target_link_libraries(hello_window glfw3 opengl32 gdi32)
