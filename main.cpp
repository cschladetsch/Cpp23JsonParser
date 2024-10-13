#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <filesystem>
#include "json.hpp"
#include "json_parser.hpp"

namespace fs = std::filesystem;
using nlohmann_json = nlohmann::json;

void benchmark_custom(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    std::cout << "Parsing file: " << filename << " (size: " << content.size() << " bytes)" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    try {
        custom_json::Value result = custom_json::parse(content);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << filename << ": " << duration.count() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing " << filename << ": " << e.what() << std::endl;
    }
}

void benchmark_nlohmann(const std::string& filename) {
    std::ifstream file(filename);
    
    auto start = std::chrono::high_resolution_clock::now();
    try {
        nlohmann_json j = nlohmann_json::parse(file);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << filename << ": " << duration.count() << std::endl;
    } catch (const nlohmann_json::parse_error& e) {
        std::cerr << "Error parsing " << filename << ": " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <custom|nlohmann> <json_directory_path>" << std::endl;
        return 1;
    }

    std::string parser_type = argv[1];
    std::string directory_path = argv[2];

    for (const auto & entry : fs::directory_iterator(directory_path)) {
        if (entry.path().extension() == ".json") {
            if (parser_type == "custom") {
                benchmark_custom(entry.path().string());
            } else if (parser_type == "nlohmann") {
                benchmark_nlohmann(entry.path().string());
            } else {
                std::cerr << "Invalid parser type. Use 'custom' or 'nlohmann'." << std::endl;
                return 1;
            }
        }
    }

    return 0;
}
