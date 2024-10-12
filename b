#!/bin/bash

# Build the project
echo "===> Building the project..."
./r

# Run the benchmark executable if build is successful
if [ $? -eq 0 ]; then
    echo "===> Running benchmarks..."
    ./build/benchmark
else
    echo "===> Build failed. Exiting."
    exit 1
fi

