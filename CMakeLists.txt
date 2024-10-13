cmake_minimum_required(VERSION 3.10)
project(Cpp23Json)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download json.hpp
file(DOWNLOAD
     https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
     ${CMAKE_BINARY_DIR}/json.hpp
)

add_executable(Cpp23Json main.cpp json_parser.cpp)
target_include_directories(Cpp23Json PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(Cpp23Json PRIVATE stdc++fs)
