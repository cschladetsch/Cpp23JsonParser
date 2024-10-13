#include <iostream>
#include <fstream>
#include "./nhomann/json.hpp" // Include nlohmann JSON
#include "./json_parser.hpp" // Include your custom JSON parser

using json = nlohmann::json;

void parse_with_nlohmann(const std::string& file_path) {
    std::ifstream file(file_path);
    json j;
    file >> j;
    // Do something with the parsed JSON
    std::cout << "Parsed with nlohmann JSON: " << j.dump().substr(0, 50) << std::endl;
}

void parse_with_custom_parser(const std::string& file_path) {
    // Call your custom parser
    std::ifstream file(file_path);
    custom_json::Value result = custom_json::parse(file);
    // Do something with the parsed JSON
    std::cout << "Parsed with Custom JSON Parser: " << result << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " --parser [custom|nlohmann] <file.json>" << std::endl;
        return 1;
    }

    std::string parser_choice = argv[1];
    std::string file_path = argv[2];

    if (parser_choice == "--parser" && argc > 2) {
        std::string parser_type = argv[2];

        if (parser_type == "nlohmann") {
            parse_with_nlohmann(file_path);
        } else if (parser_type == "custom") {
            parse_with_custom_parser(file_path);
        } else {
            std::cerr << "Unknown parser type: " << parser_type << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Invalid arguments." << std::endl;
        return 1;
    }

    return 0;
}
