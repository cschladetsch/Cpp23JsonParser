#include "json_parser.hpp"
#include "rang.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
#include <chrono>

using namespace std;

void benchmark_custom_parser() {
    auto start = chrono::high_resolution_clock::now();

    // Custom JSON parsing
    std::string json_string = R"({
        "name": "Alice",
        "age": 30,
        "is_student": false
    })";
    json::Value parsed = json::Parser::parse(json_string);

    auto end = chrono::high_resolution_clock::now();
    cout << rang::fg::green << "Custom JSON parser took: "
         << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " microseconds" << rang::style::reset << endl;
}

void benchmark_nlohmann_parser() {
    auto start = chrono::high_resolution_clock::now();

    // nlohmann JSON parsing
    std::string json_string = R"({
        "name": "Alice",
        "age": 30,
        "is_student": false
    })";
    auto parsed = nlohmann::json::parse(json_string);

    auto end = chrono::high_resolution_clock::now();
    cout << rang::fg::yellow << "nlohmann::json parser took: "
         << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " microseconds" << rang::style::reset << endl;
}

int main() {
    cout << rang::fg::blue << "Starting JSON benchmarks..." << rang::style::reset << endl;
    
    benchmark_custom_parser();
    benchmark_nlohmann_parser();

    return 0;
}
