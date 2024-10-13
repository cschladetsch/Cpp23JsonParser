import os
import json
import time
import argparse
import statistics
import subprocess
from termcolor import colored

# Directory where JSON files are stored
json_dir = "./test-json"
cpp_executable = "./build/Cpp23Json"  # Path to your C++ executable (change this if needed)

# Check if the directory contains any JSON files
try:
    files = os.listdir(json_dir)
    if not files:
        print(colored(f"No JSON files found in {json_dir}.", "red"))
        exit(1)
    print(f"Found {len(files)} JSON files.")
except Exception as e:
    print(f"Error accessing {json_dir}: {e}")
    exit(1)

# Function to benchmark the C++ JSON parser
def benchmark_cpp_json_file(file_path):
    start_time = time.time()
    try:
        # Call the C++ executable to process the JSON file
        result = subprocess.run([cpp_executable, file_path], capture_output=True, text=True)
        if result.returncode != 0:
            print(colored(f"Error running C++ parser on {file_path}: {result.stderr}", "red"))
            return 0
    except Exception as e:
        return 0
    end_time = time.time()
    return (end_time - start_time) * 1000  # Return time in milliseconds

# Function to benchmark the Python JSON parser
def benchmark_python_json_file(file_path):
    start_time = time.time()
    try:
        with open(file_path, 'r') as f:
            data = json.load(f)  # Use Python's json module
    except Exception as e:
        return 0
    end_time = time.time()
    return (end_time - start_time) * 1000  # Return time in milliseconds

# Run benchmarks for a specified duration and collect timings
def run_benchmarks(duration_seconds=90, parser='cpp'):
    times = []
    start_time = time.time()

    while (time.time() - start_time) < duration_seconds:
        for file_name in os.listdir(json_dir):
            if file_name.endswith(".json"):
                file_path = os.path.join(json_dir, file_name)
                if parser == 'cpp':
                    elapsed_time = benchmark_cpp_json_file(file_path)
                elif parser == 'python':
                    elapsed_time = benchmark_python_json_file(file_path)

                if elapsed_time > 0:
                    times.append(elapsed_time)
        
        if (time.time() - start_time) >= duration_seconds:
            break

    return times

# Function to calculate and display benchmark results in colorful ASCII format (summary only)
def display_summary(times, parser_type):
    if len(times) == 0:
        print(colored(f"No valid timings collected for {parser_type} parser.", "red"))
        return

    # Calculate mean and standard deviation
    mean = statistics.mean(times)
    stddev = statistics.stdev(times)

    # Display results with fancy colors and symbols
    print(colored(f"\n{parser_type.capitalize()} JSON Parser Benchmark Summary:", "green", attrs=["bold"]))
    
    print(f"{colored('Mean:', 'green', attrs=['bold'])} {colored(f'{mean:.2f} ms', 'green')}")
    print(f"{colored('Standard Deviation:', 'red', attrs=['bold'])} {colored(f'{stddev:.2f} ms', 'red')}")
    
    # Show additional useful info: Min, Max, Range
    min_time = min(times)
    max_time = max(times)
    print(colored("Additional Information:", "magenta", attrs=["bold"]))
    print(f"Min Time: {colored(f'{min_time:.2f} ms', 'blue')}")
    print(f"Max Time: {colored(f'{max_time:.2f} ms', 'cyan')}")
    print(f"Time Range: {colored(f'{max_time - min_time:.2f} ms', 'magenta')}")

# Main function to parse arguments and run the benchmark
if __name__ == "__main__":
    print("Starting script...")  # Immediate check

    parser = argparse.ArgumentParser(description="Run JSON benchmark for C++ and Python parsers.")
    parser.add_argument("--duration", type=int, default=90, help="Duration of the benchmark in seconds.")
    parser.add_argument("--parser", type=str, default="cpp", choices=["cpp", "python"], help="Select the parser to benchmark ('cpp' or 'python').")
    
    args = parser.parse_args()

    print(f"Running benchmark for {args.parser.capitalize()} JSON parser for {args.duration} seconds...")

    try:
        times = run_benchmarks(args.duration, args.parser)
        display_summary(times, args.parser)
    except Exception as e:
        print(f"An error occurred: {e}")
