cmake_minimum_required(VERSION 3.10)

# Set GCC as the compiler
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(Cpp23Json)

set(CMAKE_CXX_STANDARD 20)

# Add your main project executable
add_executable(Cpp23Json main.cpp json_parser.cpp)

# Add the test executable
add_executable(tests test_main.cpp)

# Include directories for nlohmann JSON and Catch2
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/path_to_catch)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/path_to_nlohmann_json)

# Enable testing with CMake
enable_testing()
add_test(NAME JSONTest COMMAND tests)

