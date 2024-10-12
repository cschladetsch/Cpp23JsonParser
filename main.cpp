#include "json_parser.hpp"
#include <iostream>

int main() {
    std::string json_str = R"(
        {
            "name": "Alice Smith",
            "age": 28,
            "is_student": true,
            "courses": ["Math", "Physics", "Computer Science"],
            "scores": {
                "Math": 95,
                "Physics": 88,
                "Computer Science": 92
            }
        }
    )";

    try {
        auto parsed = json::Parser::parse(json_str);
        const auto& obj = parsed.get<json::Object>();
        
        std::cout << "Name: " << obj.at("name").get<std::string>() << std::endl;
        std::cout << "Age: " << obj.at("age").get<double>() << std::endl;
        std::cout << "Is student: " << (obj.at("is_student").get<bool>() ? "Yes" : "No") << std::endl;

        std::cout << "Courses: ";
        for (const auto& course : obj.at("courses").get<json::Array>()) {
            std::cout << course.get<std::string>() << " ";
        }
        std::cout << std::endl;

        std::cout << "Scores:" << std::endl;
        for (const auto& [course, score] : obj.at("scores").get<json::Object>()) {
            std::cout << "  " << course << ": " << score.get<double>() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
