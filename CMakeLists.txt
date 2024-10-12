cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Cpp23Json VERSION 1.0)

# Set the C and C++ compilers to gcc and g++
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the sources for the project
add_executable(json_parser main.cpp json_parser.cpp)

# Add the sources for the test executable
add_executable(test_json_parser test_json_parser.cpp json_parser.cpp)

# Include directories for any headers (if necessary, add your own paths here)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link pthread for threading support
find_package(Threads REQUIRED)
target_link_libraries(test_json_parser PRIVATE Threads::Threads)

# Optional: Enable extra warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Optional: If you want to enable debugging flags
# set(CMAKE_BUILD_TYPE Debug)

