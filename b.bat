@echo off
:: Build and run script for Cpp23Json project (Windows version)

:: Step 1: Check for Clang or GCC
where clang >nul 2>nul
if %errorlevel% neq 0 (
    echo Clang is required but not installed. Please install Clang and try again.
    exit /b 1
)

:: Step 2: Download rang.hpp
echo Downloading rang.hpp...
if not exist third_party\rang mkdir third_party\rang
curl -s -o third_party\rang\rang.hpp https://raw.githubusercontent.com/agauniyal/rang/master/include/rang.hpp

:: Step 3: Remove old build directory and recreate it
echo Removing existing build directory...
rmdir /S /Q build
mkdir build
cd build

:: Step 4: Run CMake to configure the project
echo Running CMake...
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..

:: Step 5: Build the project
echo Building the project...
cmake --build .

:: Step 6: Run the benchmark
echo Running benchmarks...
benchmark.exe

