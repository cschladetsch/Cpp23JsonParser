# Cpp23Json Project

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Setup and Installation](#setup-and-installation)
- [Building the Project](#building-the-project)
- [Running Benchmarks](#running-benchmarks)
  - [Customizing Benchmark Duration and Generating Pie Chart](#customizing-benchmark-duration-and-generating-pie-chart)
- [Generating Test JSON Files](#generating-test-json-files)
- [Code Overview](#code-overview)
  - [C++ Code](#c-code)
    - [CMake Configuration](#cmake-configuration)
    - [Main Executable](#main-executable)
    - [JSON Parser](#json-parser)
  - [Python Benchmark Script](#python-benchmark-script)
- [Dependencies](#dependencies)
- [Notes and Preferences](#notes-and-preferences)
- [License](#license)

## Introduction

Cpp23Json is a C++20 project that includes a custom JSON parser and benchmarking tools to evaluate its performance. The project utilizes a set of 50 diverse JSON files for testing and benchmarking purposes. Additionally, a Python script is provided to run benchmarks, calculate mean and standard deviation, and optionally generate a pie chart of the results.

## Prerequisites

- **Operating System**: Linux (tested on Ubuntu 20.04 via WSL)
- **Compiler**: GCC 13.1.0 (configured to use GCC instead of Clang)
- **CMake**: Version 3.10 or higher
- **Python**: Version 3.x
- **Python Packages**:
  - `termcolor` (for colorful console output)
  - `matplotlib` (for generating pie charts)
- **Libraries**:
  - `nlohmann/json.hpp` (for JSON parsing in benchmarks)
  - **Catch2** (optional, if using for testing)

## Project Structure

```
Cpp23Json/
├── CMakeLists.txt
├── main.cpp
├── json_parser.cpp
├── json_parser.hpp
├── test-json/            # Directory containing 50 JSON test files
├── scripts/
│   ├── r                 # Build script for C++
│   ├── b                 # Build and run benchmarks script
│   └── pyb.py            # Python benchmark script
└── README.md
```

## Setup and Installation

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/cschladetsch/Cpp23Json.git
   cd Cpp23Json
   ```

2. **Install Required Packages**:

   - **For C++**:

     Make sure GCC is installed:

     ```bash
     sudo apt update
     sudo apt install build-essential cmake
     ```

   - **For Python**:

     Install required Python packages:

     ```bash
     pip install termcolor matplotlib
     ```

3. **Install Additional Libraries**:

   - **nlohmann/json.hpp**:

     Download and place `json.hpp` in the project directory or install via package manager:

     ```bash
     sudo apt install nlohmann-json3-dev
     ```

   - **Catch2** (Optional):

     If you plan to use Catch2 for testing, include it in your project.

## Building the Project

The project uses custom build scripts to simplify the building process.

1. **Build the C++ Project**:

   The `r` script is used to remove any existing build directory, create a new one, and build the project using CMake.

   ```bash
   ./r
   ```

   This script performs the following steps:

   - Removes the existing `build` directory.
   - Creates a new `build` directory.
   - Runs CMake configuration.
   - Builds the project using `make`.

2. **Build and Run Benchmarks**:

   The `b` script builds the project and runs the benchmarks.

   ```bash
   ./b
   ```

   This script:

   - Builds the project (similar to `./r`).
   - Runs the `benchmark` executable (ensure it's defined in `CMakeLists.txt`).

## Running Benchmarks

### Using the Python Benchmark Script (`pyb.py`)

The `pyb.py` script runs benchmarks on the JSON parser, calculates mean and standard deviation, and displays results with fancy colors and symbols. It can also generate a pie chart of the results.

1. **Run the Benchmark with Default Settings**:

   ```bash
   python pyb.py
   ```

   - Runs the benchmark for 90 seconds.
   - Displays mean and standard deviation.

2. **Customizing Benchmark Duration and Generating Pie Chart**

   You can specify the duration and generate a pie chart using command-line arguments.

   ```bash
   python pyb.py --duration 120 --piechart
   ```

   - `--duration`: Sets the benchmark duration in seconds (e.g., 120 seconds).
   - `--piechart`: Generates a pie chart (`benchmark_results.bmp`) showing the mean and standard deviation.

   **Example**:

   ```bash
   python pyb.py --duration 60 --piechart
   ```

## Generating Test JSON Files

The project uses 50 JSON files with different schemas for benchmarking.

1. **Generate JSON Files Using Python**:

   Use the following script to generate 50 JSON files with diverse schemas and save them in the `test-json` directory.

   ```python
   import json
   import os
   import random

   # Create a directory to store the JSON files
   output_dir = "./test-json"
   os.makedirs(output_dir, exist_ok=True)

   # Function to generate random JSON data
   def generate_random_json(schema_type, index):
       if schema_type == 1:
           return {
               "id": index,
               "name": f"test_file_{index}",
               "active": random.choice([True, False]),
               "scores": [random.randint(0, 100) for _ in range(5)]
           }
       elif schema_type == 2:
           return {
               "person": {
                   "first_name": f"FirstName_{index}",
                   "last_name": f"LastName_{index}",
                   "age": random.randint(18, 80),
                   "email": f"test_{index}@example.com"
               },
               "transactions": [{"amount": random.uniform(10.0, 500.0), "currency": "USD"} for _ in range(3)]
           }
       elif schema_type == 3:
           return {
               "product": {
                   "id": index,
                   "name": f"Product_{index}",
                   "price": random.uniform(10.0, 100.0),
                   "in_stock": random.choice([True, False])
               },
               "reviews": [{"user": f"User_{i}", "rating": random.randint(1, 5)} for i in range(5)]
           }
       elif schema_type == 4:
           return {
               "event": {
                   "title": f"Event_{index}",
                   "location": f"Location_{random.randint(1, 50)}",
                   "participants": [f"Participant_{i}" for i in range(random.randint(5, 20))],
                   "date": f"2024-{random.randint(1, 12):02}-{random.randint(1, 28):02}"
               }
           }
       else:
           return {
               "company": {
                   "name": f"Company_{index}",
                   "employees": random.randint(50, 1000),
                   "departments": [{"name": f"Department_{i}", "budget": random.uniform(10000.0, 50000.0)} for i in range(5)]
               }
           }

   # Generate 50 JSON files with different schemas
   for i in range(1, 51):
       schema_type = random.randint(1, 5)
       json_data = generate_random_json(schema_type, i)
       file_path = os.path.join(output_dir, f"test_file_{i}.json")
       with open(file_path, "w") as json_file:
           json.dump(json_data, json_file, indent=4)
   ```

   Run the script:

   ```bash
   python generate_json_files.py
   ```

## Code Overview

### C++ Code

#### CMake Configuration

The `CMakeLists.txt` file is configured to use GCC as the compiler and includes settings for building the main executable and optional test executables.

```cmake
cmake_minimum_required(VERSION 3.10)

# Force CMake to use GCC and G++
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(Cpp23Json)

set(CMAKE_CXX_STANDARD 20)

# Add your main project executable
add_executable(Cpp23Json main.cpp json_parser.cpp)

# Enable testing (if using Catch2 or other frameworks)
# enable_testing()
# add_test(NAME JSONTest COMMAND tests)
```

#### Main Executable

- **`main.cpp`**: Entry point of the application.
- **Usage**: Implement the logic to parse and handle JSON data using the custom parser.

#### JSON Parser

- **`json_parser.cpp` and `json_parser.hpp`**: Contains the implementation of the custom JSON parser.
- **Warning Notes**: Be aware of warnings such as returning references to temporary variables. Ensure proper memory management.

### Python Benchmark Script

#### `pyb.py`

- **Purpose**: Runs benchmarks on the JSON parser, calculates mean and standard deviation, and optionally generates a pie chart.
- **Usage**:

  ```bash
  python pyb.py [--duration SECONDS] [--piechart]
  ```

- **Arguments**:
  - `--duration`: Duration of the benchmark in seconds (default is 90).
  - `--piechart`: Include this flag to generate a pie chart in BMP format.

- **Features**:
  - Parses all JSON files in the `test-json` directory.
  - Calculates and displays mean and standard deviation.
  - Outputs results with colorful text and symbols.
  - Generates a pie chart (`benchmark_results.bmp`) when `--piechart` is used.

## Dependencies

- **GCC**: GNU Compiler Collection for compiling C++ code.
- **CMake**: Build system generator.
- **Python Packages**:
  - `termcolor`: For colorful console output.
  - `matplotlib`: For generating pie charts.
- **nlohmann/json.hpp**: JSON for Modern C++ library.
- **Catch2** (Optional): C++ testing framework.

## Notes and Preferences

- **Compiler Preference**: GCC is preferred over Clang. The compiler is specified in `CMakeLists.txt` to avoid setting environment variables.
- **Code Standards**: C++20 standard is used.
- **Include Guards**: Prefer using `#pragma once` for include guards.
- **Code Practices**:
  - Avoid using raw pointers.
  - Be cautious of returning references to temporary variables.
  - Use proper memory management techniques.
- **Spelling**: Australian spelling is preferred in code comments and documentation (e.g., `organise` instead of `organize`).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
