#include "json_parser.hpp"

namespace custom_json {

Value parse(const std::string& json_string) {
    // This is a placeholder implementation
    // You should replace this with your actual JSON parsing logic
    if (json_string.empty()) {
        return Value();
    }
    if (json_string == "true") {
        return Value(true);
    }
    if (json_string == "false") {
        return Value(false);
    }
    if (json_string[0] == '"') {
        return Value(json_string.substr(1, json_string.length() - 2));
    }
    try {
        return Value(std::stod(json_string));
    } catch (...) {
        // If it's not a number, return it as a string
        return Value(json_string);
    }
}

} // namespace custom_json
