#define CATCH_CONFIG_MAIN
#include <fstream>
#include <filesystem>  // C++17 feature for file system operations

#include "catch.hpp"  // Include the Catch2 header
#include "nhomann/json.hpp"  // Include your JSON library

namespace fs = std::filesystem;

// Helper function to test each JSON file
void test_json_file(const std::string& file_path) {
    std::ifstream json_file(file_path);
    REQUIRE(json_file.is_open());  // Catch2 assertion to check if file opened successfully

    nlohmann::json json_data;
    try {
        json_file >> json_data;

        // Example: Validate the JSON structure by checking some basic keys
        if (json_data.contains("test_number")) {
            REQUIRE(json_data["test_number"].is_number());
        }

        // You can add more validation here depending on your schemas
    } catch (const std::exception& e) {
        FAIL("Error processing JSON file " << file_path << ": " << e.what());
    }
}

// Catch2 test case to test all JSON files in the directory
TEST_CASE("Test all JSON files in ./test-json folder") {
    std::string folder_path = "./test-json";

    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            test_json_file(entry.path().string());
        }
    }
}

