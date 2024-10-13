#!/usr/bin/env python3

import os
import subprocess
import shutil
from termcolor import colored

def run_command(command, error_message):
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError:
        print(colored(error_message, 'red'))
        exit(1)

def main():
    # Clean
    print(colored("Cleaning build directory...", 'yellow'))
    if os.path.exists('build'):
        shutil.rmtree('build')

    # Create build directory
    print(colored("Creating build directory...", 'yellow'))
    os.mkdir('build')
    os.chdir('build')

    # CMake
    print(colored("Running CMake...", 'yellow'))
    run_command('cmake ..', "CMake configuration failed")

    # Make
    print(colored("Building project...", 'yellow'))
    run_command('make', "Build failed")

    # Change back to project root
    os.chdir('..')

    # Run benchmark
    print(colored("Running benchmark...", 'yellow'))
    run_command('python benchmark.py', "Benchmark failed")

    print(colored("All tasks completed successfully!", 'green'))

if __name__ == "__main__":
    main()
