#!/bin/bash

# Download rang.hpp if it does not exist
if [ ! -f third_party/rang/rang.hpp ]; then
    echo "===> Downloading rang.hpp..."
    mkdir -p third_party/rang
    wget https://raw.githubusercontent.com/agauniyal/rang/master/include/rang.hpp -O third_party/rang/rang.hpp
    if [ $? -ne 0 ]; then
        echo "===> Failed to download rang.hpp. Exiting."
        exit 1
    fi
fi

# Remove existing build directory
echo "===> Removing existing build directory..."
rm -rf build

# Create new build directory
echo "===> Creating build directory..."
mkdir build
cd build

# Set compilers to gcc and g++
export CC=gcc
export CXX=g++

# Run CMake
echo "===> Running CMake..."
cmake ..

# Build the project
echo "===> Building the project..."
make

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "===> Build successful!"
else
    echo "===> Build failed. Exiting."
    exit 1
fi

# Return to root directory
cd ..

# Run the benchmarks
if [ -f build/benchmark ]; then
    echo "===> Running benchmarks..."
    ./build/benchmark
else
    echo "===> Benchmark executable not found. Skipping benchmark run."
fi
