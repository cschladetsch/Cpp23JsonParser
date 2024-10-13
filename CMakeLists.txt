cmake_minimum_required(VERSION 3.10)
project(Cpp23Json)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -mtune=native -ffast-math -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -march=native -mtune=native -ffast-math -flto")

# Download json.hpp
file(DOWNLOAD
     https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
     ${CMAKE_BINARY_DIR}/json.hpp
)

# Enable assembly
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_ASM_NASM_FLAGS "-f elf64")

add_executable(Cpp23Json main.cpp json_parser.cpp fast_functions.asm)
target_include_directories(Cpp23Json PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(Cpp23Json PRIVATE stdc++fs)
