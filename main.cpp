#include "json_parser.hpp"
#include <iostream>
#include <string>

int main() {
    // Example JSON string
    std::string json_string = R"({
        "name": "Alice",
        "age": 30,
        "is_student": false
    })";

    try {
        // Parse the JSON string
        json::Value parsed = json::Parser::parse(json_string);

        // Retrieve the object (json::Value::Object type)
        const auto& obj = parsed.get<json::Value::Object>();

        // Access values in the object
        std::cout << "Name: " << obj.at("name").get<std::string>() << std::endl;
        std::cout << "Age: " << obj.at("age").get<double>() << std::endl;
        std::cout << "Is student: " << (obj.at("is_student").get<bool>() ? "Yes" : "No") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

