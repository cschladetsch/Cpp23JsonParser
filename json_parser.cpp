#include "json_parser.hpp"
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <immintrin.h>

extern "C" int fast_strcmp(const char* s1, const char* s2);

namespace custom_json {

static inline const char* skip_whitespace(const char* start, const char* end) {
    __m256i spaces = _mm256_set1_epi8(' ');
    while (start + 32 <= end) {
        __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(start));
        int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, spaces));
        if (mask != 0xFFFFFFFF) {
            return start + __builtin_ctz(~mask);
        }
        start += 32;
    }
    while (start < end && std::isspace(*start)) ++start;
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
    throw std::runtime_error("Invalid number");
}

static Value parse_array(const char*& start, const char* end);
static Value parse_object(const char*& start, const char* end);

static Value parse_value(const char*& start, const char* end) {
    start = skip_whitespace(start, end);
    
    switch (*start) {
        case 'n':
            if (end - start >= 4 && fast_strcmp(start, "null") == 0) {
                start += 4;
                return Value();
            }
            break;
        case 't':
            if (end - start >= 4 && fast_strcmp(start, "true") == 0) {
                start += 4;
                return Value(true);
            }
            break;
        case 'f':
            if (end - start >= 5 && fast_strcmp(start, "false") == 0) {
                start += 5;
                return Value(false);
            }
            break;
        case '"':
            return parse_string(start, end);
        case '[':
            return parse_array(start, end);
        case '{':
            return parse_object(start, end);
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return parse_number(start, end);
    }
    
    throw std::runtime_error("Invalid JSON value");
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
    ++start; // Skip opening brace
    start = skip_whitespace(start, end);
    
    if (*start != '}') {
        do {
            if (*start != '"') throw std::runtime_error("Expected string as key in object");
            std::string key = parse_string(start, end).as_string();
            
            start = skip_whitespace(start, end);
            if (*start != ':') throw std::runtime_error("Expected ':' after key in object");
            ++start;
            
            obj[std::move(key)] = parse_value(start, end);
            start = skip_whitespace(start, end);
        } while (*start == ',' && (++start, true));
        
        if (*start != '}') throw std::runtime_error("Expected '}' in object");
    }
    
    ++start; // Skip closing brace
    return Value(std::move(obj));
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
