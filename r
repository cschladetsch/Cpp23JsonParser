#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Function to print messages
print_message() {
    echo "===> $1"
}

# Function to print errors
print_error() {
    echo "ERROR: $1" >&2
}

# Ensure we're in the project root directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
fi

# Check if all necessary files exist
for file in "main.cpp" "json_parser.cpp" "json_parser.hpp" "test_json_parser.cpp"; do
    if [ ! -f "$file" ]; then
        print_error "$file not found. Please ensure all source files are present."
        exit 1
    fi
done

# Remove existing build directory
print_message "Removing existing build directory..."
rm -rf build

# Create and enter build directory
print_message "Creating build directory..."
mkdir -p build
cd build

# Run CMake with explicit g++ compiler
print_message "Running CMake..."
if ! CXX=g++ cmake ..; then
    print_error "CMake configuration failed. Please check your CMakeLists.txt for errors."
    exit 1
fi

# Build the project
print_message "Building the project..."
if ! make -j$(nproc); then
    print_error "Build failed. Please check the compiler output for errors."
    exit 1
fi

# Check if the executables were created
for exec in "json_parser" "test_json_parser"; do
    if [ ! -f "$exec" ]; then
        print_error "The executable '$exec' was not created. Build may have failed silently."
        exit 1
    fi
done

# Run the tests
print_message "Running tests..."
if ! ./test_json_parser; then
    print_error "Some tests failed. Please check the test output for details."
    exit 1
fi

# Run the main executable
print_message "Running the JSON parser..."
./json_parser

# Return to the original directory
cd ..

print_message "Script completed successfully!"
