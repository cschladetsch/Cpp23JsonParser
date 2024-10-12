cmake_minimum_required(VERSION 3.10)

project(Cpp23Json VERSION 1.0)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library
add_library(json_parser_lib json_parser.cpp)

# Add the executables
add_executable(json_parser_driver main.cpp)
add_executable(test_json_parser test_json_parser.cpp)

# Link the library with the executables
target_link_libraries(json_parser_driver PRIVATE json_parser_lib)
target_link_libraries(test_json_parser PRIVATE json_parser_lib)

# Enable testing with Catch2
enable_testing()
add_test(NAME json_parser_test COMMAND test_json_parser)

# Optional: Extra warnings
add_compile_options(-Wall -Wextra -Wpedantic)

