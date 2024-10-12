cmake_minimum_required(VERSION 3.20)

# Explicitly set the C++ compiler to g++
set(CMAKE_CXX_COMPILER g++)

project(FastJsonParser VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Add the main executable
add_executable(json_parser
    main.cpp
    json_parser.cpp
)

# Add the test executable
add_executable(test_json_parser
    test_json_parser.cpp
    json_parser.cpp
)

# Install the targets (optional)
install(TARGETS json_parser test_json_parser DESTINATION bin)

# Enable testing
enable_testing()
add_test(NAME JsonParserTests COMMAND test_json_parser)
