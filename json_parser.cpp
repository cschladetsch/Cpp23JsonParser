#include "json_parser.hpp"
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <immintrin.h>

extern "C" int fast_strcmp(const char* s1, const char* s2);

namespace custom_json {

static Value parse_array(const char*& start, const char* end);
static Value parse_object(const char*& start, const char* end);

static const char* skip_whitespace(const char*& start, const char* end) {
    // Skip all whitespace characters, including newlines, spaces, tabs, etc.
    while (start < end && (isspace(*start) || *start == '\n' || *start == '\r' || *start == '\t')) {
        ++start;
    }
    return start;
}

static Value parse_string(const char*& start, const char* end) {
    ++start; // Skip opening quote
    const char* str_end = start;
    while (str_end < end) {
        if (*str_end == '"' && *(str_end - 1) != '\\') break;
        ++str_end;
    }
    if (str_end < end) {
        Value result(std::string(start, str_end));
        start = str_end + 1; // Skip closing quote
        return result;
    }
    throw std::runtime_error("Unterminated string");
}

static Value parse_number(const char*& start, const char* end) {
    char* num_end;
    double num = std::strtod(start, &num_end);
    if (num_end > start) {
        start = num_end;
        return Value(num);
    }
    throw std::runtime_error("Invalid number: " + std::string(start, end - start));
}

static Value parse_value(const char*& start, const char* end) {
    start = skip_whitespace(start, end);  // Ensure we skip any leading whitespace

    if (start >= end) {
        throw std::runtime_error("Unexpected end of JSON");
    }

    switch (*start) {
        case 'n':  // Parse `null`
            if (end - start >= 4 && strncmp(start, "null", 4) == 0) {
                start += 4;
                return Value();  // Return null
            }
            break;
        case 't':  // Parse `true`
            if (end - start >= 4 && strncmp(start, "true", 4) == 0) {
                start += 4;
                return Value(true);  // Return boolean true
            }
            break;
        case 'f':  // Parse `false`
            if (end - start >= 5 && strncmp(start, "false", 5) == 0) {
                start += 5;
                return Value(false);  // Return boolean false
            }
            break;
        case '"':  // Parse string
            return parse_string(start, end);
        case '[':  // Parse array
            return parse_array(start, end);
        case '{':  // Parse object
            return parse_object(start, end);
        case '-':  // Parse number (negative)
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':  // Parse number
            return parse_number(start, end);
        default:
            throw std::runtime_error("Unrecognized JSON value");
    }

    throw std::runtime_error("Unrecognized JSON value");
}

static Value parse_array(const char*& start, const char* end) {
    Value::Array arr;
    ++start; // Skip opening bracket
    start = skip_whitespace(start, end);
    
    if (*start != ']') {
        do {
            arr.push_back(parse_value(start, end));
            start = skip_whitespace(start, end);
        } while (*start == ',' && (++start, true));
        
        if (*start != ']') throw std::runtime_error("Expected ']' in array");
    }
    
    ++start; // Skip closing bracket
    return Value(std::move(arr));
}

static Value parse_object(const char*& start, const char* end) {
    Value::Object obj;
    ++start;  // Skip the opening brace '{'
    start = skip_whitespace(start, end);

    if (*start != '}') {  // If the object is not immediately closed
        do {
            // Skip any leading whitespace before parsing the key
            start = skip_whitespace(start, end);

            // If the first non-whitespace character is not a double-quote, throw an error
            if (*start != '"') {
                std::cerr << "Error: Expected string as key in object, but found '" << *start << "' instead." << std::endl;
                throw std::runtime_error("Expected string as key in object");
            }

            // Parse the string key
            std::string key = parse_string(start, end).as_string();
            //std::cout << "Parsed key: " << key << std::endl;  // Debugging: log the parsed key

            // Skip whitespace after the key
            start = skip_whitespace(start, end);

            // Ensure the colon ':' follows the key
            if (*start != ':') {
                std::cerr << "Error: Expected ':' after key in object, but found '" << *start << "' instead." << std::endl;
                throw std::runtime_error("Expected ':' after key in object");
            }

            ++start;  // Skip the colon
            start = skip_whitespace(start, end);  // Skip whitespace after the colon

            // Parse the associated value
            obj[key] = parse_value(start, end);

            // Skip any trailing whitespace and check for a comma or closing brace
            start = skip_whitespace(start, end);
        } while (*start == ',' && (++start, true));  // Move to the next key-value pair if a comma is present

        // Ensure the object is properly closed
        if (*start != '}') {
            std::cerr << "Error: Expected '}' at the end of object, but found '" << *start << "' instead." << std::endl;
            throw std::runtime_error("Expected '}' at the end of object");
        }
    }

    ++start;  // Skip the closing brace '}'
    return Value(std::move(obj));  // Return the parsed object
}

Value parse(const std::string& json_string) {
    const char* start = json_string.c_str();
    const char* end = start + json_string.length();
    try {
        Value result = parse_value(start, end);
        start = skip_whitespace(start, end);
        if (start != end) throw std::runtime_error("Unexpected trailing characters");
        return result;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("JSON parse error: ") + e.what());
    }
}

} // namespace custom_json
