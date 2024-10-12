#pragma once

#include "value.hpp"  // Include the full definition of json::Value
#include <string>
#include <cstddef>
#include <cctype> // For isspace

namespace json {

class Parser {
public:
    static Value parse_value(const char* json, size_t& pos);
    static std::string parse_string(const char* json, size_t& pos);
    static double parse_number(const char* json, size_t& pos);
    static Value parse_array(const char* json, size_t& pos);
    static Value parse_object(const char* json, size_t& pos);

    static void skip_whitespace(const char* json, size_t& pos) {
        while (std::isspace(json[pos])) {
            pos++;
        }
    }

    Value parse(const std::string& json);
};

} // namespace json

