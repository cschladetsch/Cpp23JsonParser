#include "json_parser.hpp"
#include <stdexcept>
#include <cctype>      // for isdigit
#include <cstring>     // for strncmp

namespace json {

// Forward declaration of helper functions
static Value parse_value(const char* json, size_t& pos);
static std::string parse_string(const char* json, size_t& pos);
static double parse_number(const char* json, size_t& pos);
static Value parse_array(const char* json, size_t& pos);
static Value parse_object(const char* json, size_t& pos);

// Skip whitespace
static void skip_whitespace(const char* json, size_t& pos) {
    while (std::isspace(json[pos])) {
        pos++;
    }
}

// Parse a JSON string
static std::string parse_string(const char* json, size_t& pos) {
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
static double parse_number(const char* json, size_t& pos) {
    size_t start_pos = pos;
    while (std::isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-') {
        pos++;
    }
    return std::stod(std::string(json + start_pos, pos - start_pos));
}

// Parse a JSON array
static Value parse_array(const char* json, size_t& pos) {
    if (json[pos] != '[') {
        throw std::runtime_error("Expected '[' at the beginning of array.");
    }
    pos++;  // Skip the opening bracket
    skip_whitespace(json, pos);
    Value::Array array;
    while (json[pos] != ']') {
        array.emplace_back(parse_value(json, pos));
        skip_whitespace(json, pos);
        if (json[pos] == ',') {
            pos++;
            skip_whitespace(json, pos);
        }
    }
    pos++;  // Skip the closing bracket
    return json::Value(array);
}

// Parse a JSON object
static Value parse_object(const char* json, size_t& pos) {
    if (json[pos] != '{') {
        throw std::runtime_error("Expected '{' at the beginning of object.");
    }
    pos++;  // Skip the opening bracket
    skip_whitespace(json, pos);
    Value::Object obj;
    while (json[pos] != '}') {
        std::string key = parse_string(json, pos);  // Parse the key
        skip_whitespace(json, pos);
        if (json[pos] != ':') {
            throw std::runtime_error("Expected ':' after key in object.");
        }
        pos++;  // Skip the colon
        skip_whitespace(json, pos);
        obj[key] = parse_value(json, pos);  // Parse the value associated with the key
        skip_whitespace(json, pos);
        if (json[pos] == ',') {
            pos++;
            skip_whitespace(json, pos);
        }
    }
    pos++;  // Skip the closing bracket
    return json::Value(obj);
}

// Parse a JSON value (object, array, string, number, etc.)
static Value parse_value(const char* json, size_t& pos) {
    skip_whitespace(json, pos);
    if (json[pos] == '"') {
        return json::Value(parse_string(json, pos));
    } else if (json[pos] == '{') {
        return parse_object(json, pos);
    } else if (json[pos] == '[') {
        return parse_array(json, pos);
    } else if (std::strncmp(json + pos, "null", 4) == 0) {
        pos += 4;
        return json::Value(nullptr);
    } else if (std::strncmp(json + pos, "true", 4) == 0) {
        pos += 4;
        return json::Value(true);
    } else if (std::strncmp(json + pos, "false", 5) == 0) {
        pos += 5;
        return json::Value(false);
    } else {
        return json::Value(parse_number(json, pos));
    }
}

// JSON Parser's entry point
Value Parser::parse(const std::string& json) {
    size_t pos = 0;
    return parse_value(json.c_str(), pos);
}

}  // namespace json

