import subprocess
import statistics
import json
import time
import os
from termcolor import colored

Iterations=10000

def run_cpp_benchmark(parser_type, iterations=Iterations):
    all_times = []
    for _ in range(iterations):
        result = subprocess.run(['./build/Cpp23Json', parser_type, './test-json'], 
                                capture_output=True, text=True)
        lines = result.stdout.strip().split('\n')
        times = []
        for line in lines:
            parts = line.split(': ')
            if len(parts) == 2:
                try:
                    times.append(float(parts[1]))
                except ValueError:
                    print(f"Warning: Could not parse time from line: {line}")
        all_times.extend(times)
    return all_times

def run_python_benchmark(iterations=Iterations):
    all_times = []
    for _ in range(iterations):
        times = []
        for filename in os.listdir('./test-json'):
            if filename.endswith('.json'):
                filepath = os.path.join('./test-json', filename)
                start_time = time.perf_counter()
                with open(filepath, 'r') as f:
                    json.load(f)
                end_time = time.perf_counter()
                times.append((end_time - start_time) * 1000)  # Convert to milliseconds
        all_times.extend(times)
    return all_times

def print_statistics(name, times):
    if not times:
        print(f"\n{colored(name, 'cyan', attrs=['bold'])} Parser: No valid times collected.")
        return
    print(f"\n{colored(name, 'cyan', attrs=['bold'])} Parser Statistics:")
    print(f"Mean: {colored(f'{statistics.mean(times):.6f}', 'green')} ms")
    print(f"Median: {colored(f'{statistics.median(times):.6f}', 'green')} ms")
    print(f"Min: {colored(f'{min(times):.6f}', 'green')} ms")
    print(f"Max: {colored(f'{max(times):.6f}', 'green')} ms")
    print(f"Std Dev: {colored(f'{statistics.stdev(times):.6f}', 'yellow')} ms")

if __name__ == "__main__":
    print(colored("Running benchmarks...", "yellow", attrs=["bold"]))

    custom_times = run_cpp_benchmark('custom')
    nlohmann_times = run_cpp_benchmark('nlohmann')
    python_times = run_python_benchmark()

    print_statistics("Custom C++", custom_times)
    print_statistics("Nlohmann", nlohmann_times)
    print_statistics("Python", python_times)

    if custom_times and nlohmann_times and python_times:
        custom_mean = statistics.mean(custom_times)
        nlohmann_mean = statistics.mean(nlohmann_times)
        python_mean = statistics.mean(python_times)

        print("\nSpeedup Comparisons:")
        print(f"Custom C++ vs Nlohmann: {colored(f'{nlohmann_mean/custom_mean:.2f}x', 'red', attrs=['bold'])}")
        print(f"Custom C++ vs Python: {colored(f'{python_mean/custom_mean:.2f}x', 'red', attrs=['bold'])}")
        print(f"Nlohmann vs Python: {colored(f'{python_mean/nlohmann_mean:.2f}x', 'red', attrs=['bold'])}")

        fastest = min(custom_mean, nlohmann_mean, python_mean)
        if fastest == custom_mean:
            print(colored("\nCustom C++ parser is the fastest!", 'green', attrs=['bold']))
        elif fastest == nlohmann_mean:
            print(colored("\nNlohmann parser is the fastest!", 'green', attrs=['bold']))
        else:
            print(colored("\nPython parser is the fastest!", 'green', attrs=['bold']))
    else:
        print(colored("\nUnable to determine the fastest parser due to missing data.", 'red', attrs=['bold']))
