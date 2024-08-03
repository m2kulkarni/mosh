cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Termi VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory
include_directories(include)
include_directories(/usr/include/freetype2)

# Find the required packages
find_package(SDL2 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add the source files
file(GLOB SOURCES "src/*.cpp" "src/*.c")

# Add the executable
add_executable(term ${SOURCES})

# Link the libraries
target_link_libraries(term glfw m dl Xrandr freetype glm::glm ${SDL2_LIBRARIES})
target_include_directories(term PRIVATE ${SDL2_INCLUDE_DIRS})
