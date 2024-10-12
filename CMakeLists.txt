cmake_minimum_required(VERSION 3.10)

# Force CMake to use GCC and G++
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(Cpp23Json)

set(CMAKE_CXX_STANDARD 20)

# Add your main project executable
add_executable(Cpp23Json main.cpp json_parser.cpp)

# Add benchmark executable
add_executable(benchmark benchmark.cpp json_parser.cpp)

