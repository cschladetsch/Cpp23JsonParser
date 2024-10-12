cmake_minimum_required(VERSION 3.10)

project(Cpp23Json VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add rang as an external dependency
include_directories(third_party/rang)  # Assuming you have placed it in third_party/rang


# Include FetchContent module to download and manage external dependencies
include(FetchContent)

# Download nlohmann/json using FetchContent
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2  # You can change this to the version you want
)

FetchContent_MakeAvailable(nlohmann_json)

# Add the source files for the json_parser_lib
add_library(json_parser_lib STATIC json_parser.cpp)

# Add the json parser driver executable
add_executable(json_parser_driver main.cpp)
target_link_libraries(json_parser_driver PRIVATE json_parser_lib)

# Add the test executable for unit testing
add_executable(test_json_parser test_json_parser.cpp)
target_link_libraries(test_json_parser PRIVATE json_parser_lib Catch2::Catch2)

# Benchmark executable, link against nlohmann_json
add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE json_parser_lib nlohmann_json::nlohmann_json)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add Catch2 for unit testing
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.10
)
FetchContent_MakeAvailable(Catch2)

# Link the Catch2 library to the test executable
target_link_libraries(test_json_parser PRIVATE Catch2::Catch2)

# Optional: Extra warnings
add_compile_options(-Wall -Wextra -Wpedantic)

