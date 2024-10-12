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
   

