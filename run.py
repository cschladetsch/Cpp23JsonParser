import os
import json
import time
import argparse
import statistics
import matplotlib.pyplot as plt
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

# Run benchmarks for a specified duration and collect timings
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
def display_results(times, pie_chart=False):
    if len(times) == 0:
        print(colored("No valid timings collected.", "red"))
        return

    mean = statistics.mean(times)
    stddev = statistics.stdev(times)

    # Display results with fancy colors and symbols
    print(colored("Benchmark Results:", "green", attrs=["bold"]))
    print(f"{colored('Mean:', 'green')} {colored(f'{mean:.2f} ms', 'green')} \u2713")
    print(f"{colored('Standard Deviation:', 'red')} {colored(f'{stddev:.2f} ms', 'red')} \u2717")

    # Optionally generate a pie chart
    if pie_chart:
        generate_pie_chart(mean, stddev)

# Function to generate a pie chart and save it as BMP
def generate_pie_chart(mean, stddev):
    labels = ['Mean', 'Standard Deviation']
    sizes = [mean, stddev]
    colors = ['#66b3ff', '#ff6666']
    
    plt.pie(sizes, labels=labels, colors=colors, autopct='%1.1f%%', startangle=140)
    plt.axis('equal')  # Equal aspect ratio ensures the pie chart is circular.
    
    # Save pie chart as BMP
    plt.savefig("benchmark_results.bmp", format="bmp")
    print(colored("Pie chart saved as 'benchmark_results.bmp'", "blue", attrs=["bold"]))

# Main function to parse arguments and run the benchmark
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run JSON benchmark and optionally save results as a pie chart.")
    parser.add_argument("--duration", type=int, default=90, help="Duration of the benchmark in seconds.")
    parser.add_argument("--piechart", action="store_true", help="Generate a pie chart in BMP format.")
    
    args = parser.parse_args()

    print(colored(f"Running benchmark for {args.duration} seconds...", "yellow", attrs=["bold"]))
    times = run_benchmarks(args.duration)
    display_results(times, args.piechart)
