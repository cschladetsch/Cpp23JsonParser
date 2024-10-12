#!/bin/bash
# This script handles the full build and test process.
set -e

# Download dependencies (e.g., rang.hpp) if not already present
if [ ! -f "third_party/rang/rang.hpp" ]; then
    echo "===> Downloading rang.hpp..."
    mkdir -p third_party/rang
    curl -o third_party/rang/rang.hpp https://raw.githubusercontent.com/agauniyal/rang/master/include/rang.hpp
fi

# Remove and rebuild project
echo "===> Removing existing build directory..."
rm -rf build
mkdir build

echo "===> Creating build directory..."
pushd build
cmake ..
popd
echo "===> Running CMake..."

# Build the project
echo "===> Building the project..."
pushd build
cmake --build .
popd

echo "===> Build successful!"

