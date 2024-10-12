cmake_minimum_required(VERSION 3.10)
project(Cpp23Json LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(benchmark PRIVATE nlohmann_json::nlohmann_json)


# Add the source files
set(SOURCES
    json_parser.cpp
    test_json_parser.cpp
)

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR})

# Define the Catch2 library to handle unit testing
add_library(catch_main OBJECT catch.hpp)

# Create a target for the unit tests
add_executable(test_json_parser ${SOURCES})
target_link_libraries(test_json_parser PRIVATE catch_main)

# Enable testing
enable_testing()
add_test(NAME json_tests COMMAND test_json_parser)

add_executable(benchmark benchmark.cpp)

