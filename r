#!/bin/bash

# Remove existing build directory
echo "===> Removing existing build directory..."
rm -rf build

# Create new build directory
echo "===> Creating build directory..."
mkdir build
cd build

# Run CMake from the root folder (one directory up)
echo "===> Running CMake..."
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..

# Check if CMake was successful
if [ $? -ne 0 ]; then
  echo "CMake configuration failed. Exiting."
  exit 1
fi

# Build the project
echo "===> Building the project..."
make

# Check if make was successful
if [ $? -ne 0 ]; then
  echo "Build failed. Exiting."
  exit 1
fi

# Run the tests (assuming the test binary is test_json_parser)
echo "===> Running tests..."
./test_json_parser

# Check if the tests ran successfully
if [ $? -ne 0 ]; then
  echo "Tests failed. Exiting."
  exit 1
fi

echo "Build and tests successful!"

