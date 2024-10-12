import os
import json
import time
import statistics
from termcolor import colored

# Directory where JSON files are stored
json_dir = "./test-json"

# Function to parse JSON file and return the time taken to parse it
def benchmark_json_file(file_path):
    start_time = time.time()
    try:
        with open(file_path, 'r') as f:
            data = json.load(f)
            # Simulate processing (if needed, add actual logic here)
            for key, value in data.items():
                pass
    except Exception as e:
        print(colored(f"Error processing file {file_path}: {e}", "red"))
        return 0
    end_time = time.time()
    return (end_time - start_time) * 1000  # Return time in milliseconds

# Run benchmarks for 90 seconds and collect timings
def run_benchmarks(duration_seconds=90):
    times = []
    start_time = time.time()

    while (time.time() - start_time) < duration_seconds:
        for file_name in os.listdir(json_dir):
            if file_name.endswith(".json"):
                file_path = os.path.join(json_dir, file_name)
                elapsed_time = benchmark_json_file(file_path)
                if elapsed_time > 0:
                    times.append(elapsed_time)
        
        if (time.time() - start_time) >= duration_seconds:
            break

    return times

# Function to calculate and display benchmark results
def display_results(times):
    if len(times) == 0:
        print(colored("No valid timings collected.", "red"))
        return

    mean = statistics.mean(times)
    stddev = statistics.stdev(times)

    # Display results with fancy colors and symbols
    print(colored("Benchmark Results:", "green", attrs=["bold"]))
    print(f"{colored('Mean:', 'green')} {colored(f'{mean:.2f} ms', 'green')} \u2713")
    print(f"{colored('Standard Deviation:', 'red')} {colored(f'{stddev:.2f} ms', 'red')} \u2717")

# Run the benchmark and show results
if __name__ == "__main__":
    print(colored("Running benchmark for 90 seconds...", "yellow", attrs=["bold"]))
    times = run_benchmarks(90)
    display_results(times)
