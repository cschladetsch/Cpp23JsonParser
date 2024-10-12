#!/bin/bash

# Build the project
echo "Building the project..."
cmake --build build || exit 1

# Run the benchmarks
echo "Running benchmarks..."
./build/benchmark || echo "Failed to run benchmark"

