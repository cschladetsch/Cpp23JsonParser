#pragma once

#include "value.hpp"
#include <string>
#include <cstddef>
#include <cctype>  // For isspace

namespace json {

class Parser {
public:
    Value parse(const std::string& json);  // Method to parse JSON string
    static void skip_whitespace(const char* json, size_t& pos);

private:
    Value parse_value(const char* json, size_t& pos);  // Member function to parse a value
    std::string parse_string(const char* json, size_t& pos);
    double parse_number(const char* json, size_t& pos);
    Value parse_array(const char* json, size_t& pos);
    Value parse_object(const char* json, size_t& pos);
};

}  // namespace json

