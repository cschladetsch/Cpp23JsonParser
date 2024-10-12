#include <nlohmann/json.hpp>
#include "json_parser.hpp"
#include "rang.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip> // For formatting output

const int NUM_ITERATIONS = 10000;

// Function to measure the duration of a function call
template<typename Func>
long long measure_duration(Func func, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / iterations;
}

// Test with different JSON strings
void run_benchmarks(const std::string& json_str, const std::string& test_name) {
    std::cout << rang::fg::cyan << "\nBenchmarking " << test_name << "...\n" << rang::style::reset;

    // Benchmark custom JSON parser
    long long custom_duration = measure_duration([&]() {
        json::Parser::parse(json_str);
    }, NUM_ITERATIONS);

    // Benchmark nlohmann::json parser
    long long nlohmann_duration = measure_duration([&]() {
        nlohmann::json::parse(json_str);
    }, NUM_ITERATIONS);

    // Display the results with colors
    std::cout << rang::fg::green << std::left << std::setw(30) << "Custom JSON parser"
              << rang::fg::yellow << ": " << custom_duration << " µs\n" << rang::style::reset;

    std::cout << rang::fg::green << std::left << std::setw(30) << "nlohmann::json parser"
              << rang::fg::yellow << ": " << nlohmann_duration << " µs\n" << rang::style::reset;
}

int main() {
    // JSON strings of varying complexity
    std::string simple_json = R"({"name": "Test", "value": 42})";
    std::string medium_json = R"({
        "name": "Test",
        "values": [1, 2, 3, 4, 5],
        "active": true,
        "info": {
            "id": 100,
            "description": "Benchmarking test"
        }
    })";
    std::string complex_json = R"({
        "company": "Tech Corp",
        "employees": [
            {"name": "Alice", "age": 30, "position": "Engineer"},
            {"name": "Bob", "age": 35, "position": "Manager"},
            {"name": "Charlie", "age": 25, "position": "Intern"}
        ],
        "departments": {
            "Engineering": {"head": "Alice", "budget": 500000},
            "Management": {"head": "Bob", "budget": 200000}
        },
        "locations": ["New York", "San Francisco", "Austin"]
    })";

    // Run benchmarks for each JSON string
    run_benchmarks(simple_json, "Simple JSON");
    run_benchmarks(medium_json, "Medium JSON");
    run_benchmarks(complex_json, "Complex JSON");

    return 0;
}

