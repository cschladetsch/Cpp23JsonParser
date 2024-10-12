#include "json_parser.hpp"
#include <stdexcept>
#include <cctype>
#include <cstring>

// Function declarations
static json::Value parse_value(const char* json, size_t& pos);
static json::Value parse_number(const char* json, size_t& pos);
static json::Value parse_string(const char* json, size_t& pos);
static json::Value parse_array(const char* json, size_t& pos);
static json::Value parse_object(const char* json, size_t& pos);

// Function to skip whitespace
void skip_whitespace(const char* json, size_t& pos) {
    while (std::isspace(json[pos])) {
        pos++;
    }
}

// Function to parse a number
static json::Value parse_number(const char* json, size_t& pos) {
    char* end;
    double num = std::strtod(json + pos, &end);
    pos = end - json;
    return json::Value(num);
}

// Function to parse a string
static json::Value parse_string(const char* json, size_t& pos) {
    std::string result;
    pos++; // Skip the opening quote
    while (json[pos] != '"') {
        result += json[pos++];
    }
    pos++; // Skip the closing quote
    return json::Value(result);
}

// Function to parse an array
static json::Value parse_array(const char* json, size_t& pos) {
    json::Value::Array array;
    pos++; // Skip the opening '['
    skip_whitespace(json, pos);

    while (json[pos] != ']') {
        array.emplace_back(parse_value(json, pos)); // Parse the array element
        skip_whitespace(json, pos);

        if (json[pos] == ',') {
            pos++; // Skip the comma
            skip_whitespace(json, pos);
        } else if (json[pos] != ']') {
            throw std::runtime_error("Invalid JSON input: Expected ',' or ']'");
        }
    }
    pos++; // Skip the closing ']'
    return json::Value(array);
}

// Function to parse an object
static json::Value parse_object(const char* json, size_t& pos) {
    json::Value::Object obj;
    pos++; // Skip the opening '{'
    skip_whitespace(json, pos);

    while (json[pos] != '}') {
        if (json[pos] != '"') {
            throw std::runtime_error("Invalid JSON input: Expected string key");
        }

        std::string key = parse_string(json, pos).get<std::string>();
        skip_whitespace(json, pos);

        if (json[pos] != ':') {
            throw std::runtime_error("Invalid JSON input: Expected ':' after key");
        }

        pos++; // Skip the colon
        skip_whitespace(json, pos);
        obj[key] = parse_value(json, pos); // Parse the value associated with the key
        skip_whitespace(json, pos);

        if (json[pos] == ',') {
            pos++; // Skip the comma
            skip_whitespace(json, pos);
        } else if (json[pos] != '}') {
            throw std::runtime_error("Invalid JSON input: Expected ',' or '}'");
        }
    }
    pos++; // Skip the closing '}'
    return json::Value(obj);
}

// Function to parse a value
static json::Value parse_value(const char* json, size_t& pos) {
    skip_whitespace(json, pos);

    if (json[pos] == '"') {
        return parse_string(json, pos);
    } else if (json[pos] == '{') {
        return parse_object(json, pos);
    } else if (json[pos] == '[') {
        return parse_array(json, pos);
    } else if (std::isdigit(json[pos]) || json[pos] == '-') {
        return parse_number(json, pos);
    } else if (strncmp(json + pos, "true", 4) == 0) {
        pos += 4;
        return json::Value(true);
    } else if (strncmp(json + pos, "false", 5) == 0) {
        pos += 5;
        return json::Value(false);
    } else if (strncmp(json + pos, "null", 4) == 0) {
        pos += 4;
        return json::Value(nullptr);
    } else {
        throw std::runtime_error("Invalid JSON input");
    }
}

// Parser class method to start parsing from the JSON string
json::Value json::Parser::parse(const std::string& json) {
    size_t pos = 0;
    return parse_value(json.c_str(), pos);
}

