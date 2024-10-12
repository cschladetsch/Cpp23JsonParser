#include "json_parser.hpp"
#include "value.hpp"
#include <stdexcept>
#include <cctype>
#include <cstring>
#include <vector>

namespace json {

// Skip whitespace
void Parser::skip_whitespace(const char* json, size_t& pos) {
    while (std::isspace(json[pos])) {
        pos++;
    }
}

// Parse a JSON value (null, boolean, number, string, array, or object)
Value Parser::parse_value(const char* json, size_t& pos) {
    skip_whitespace(json, pos);

    if (json[pos] == '"') {
        return parse_string(json, pos);
    } else if (std::isdigit(json[pos]) || json[pos] == '-') {
        return parse_number(json, pos);
    } else if (json[pos] == '{') {
        return parse_object(json, pos);
    } else if (json[pos] == '[') {
        return parse_array(json, pos);
    } else if (strncmp(&json[pos], "null", 4) == 0) {
        pos += 4;
        return Value(nullptr);
    } else if (strncmp(&json[pos], "true", 4) == 0) {
        pos += 4;
        return Value(true);
    } else if (strncmp(&json[pos], "false", 5) == 0) {
        pos += 5;
        return Value(false);
    } else {
        throw std::runtime_error("Invalid JSON value");
    }
}

// Parse a JSON string
std::string Parser::parse_string(const char* json, size_t& pos) {
    if (json[pos] != '"') {
        throw std::runtime_error("Expected '\"' at the beginning of string.");
    }
    pos++;  // Skip the opening quotation mark
    std::string result;
    while (json[pos] != '"') {
        result += json[pos++];
    }
    pos++;  // Skip the closing quotation mark
    return result;
}

// Parse a JSON number
double Parser::parse_number(const char* json, size_t& pos) {
    size_t start_pos = pos;
    while (std::isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-') {
        pos++;
    }
    return std::stod(std::string(json + start_pos, pos - start_pos));
}

// Parse a JSON array
Value Parser::parse_array(const char* json, size_t& pos) {
    if (json[pos] != '[') {
        throw std::runtime_error("Expected '[' at the beginning of array.");
    }
    pos++;  // Skip the opening bracket

    std::vector<Value> array;

    while (json[pos] != ']') {
        skip_whitespace(json, pos);
        array.push_back(parse_value(json, pos));  // Parse the array elements

        skip_whitespace(json, pos);
        if (json[pos] == ',') {
            pos++;  // Skip the comma between array elements
            skip_whitespace(json, pos);
        } else if (json[pos] != ']') {
            throw std::runtime_error("Expected ']' or ',' in array.");
        }
    }
    pos++;  // Skip the closing bracket

    return Value(array);
}

// Parse a JSON object
Value Parser::parse_object(const char* json, size_t& pos) {
    // TODO: Implement object parsing logic
    throw std::runtime_error("Object parsing not yet implemented.");
}

// Implement the Parser::parse method
Value Parser::parse(const std::string& json_string) {
    size_t pos = 0;
    return parse_value(json_string.c_str(), pos);
}

}  // namespace json

