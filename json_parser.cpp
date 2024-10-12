#include "json_parser.hpp"
#include <cctype>
#include <sstream>

namespace json {

    Value Parser::parse(const std::string& json_string) {
        size_t pos = 0;
        return parse_value(json_string.c_str(), pos);
    }

    void Parser::skip_whitespace(const char*& json, size_t& pos) {
        while (std::isspace(json[pos])) ++pos;
    }

    Value Parser::parse_value(const char* json, size_t& pos) {
        skip_whitespace(json, pos);

        switch (json[pos]) {
            case '{': return parse_object(json, pos);
            case '[': return parse_array(json, pos);
            case '"': return parse_string(json, pos);
            case 't': case 'f': return parse_boolean(json, pos);
            case 'n': return parse_null(json, pos);
            default: return parse_number(json, pos);
        }
    }

    Value Parser::parse_object(const char* json, size_t& pos) {
        ++pos;  // Skip '{'
        std::map<std::string, Value> obj;

        skip_whitespace(json, pos);
        while (json[pos] != '}') {
            skip_whitespace(json, pos);
            std::string key = parse_string(json, pos).as_string();
            skip_whitespace(json, pos);

            if (json[pos] != ':') throw std::runtime_error("Expected ':'");
            ++pos;  // Skip ':'

            Value value = parse_value(json, pos);
            obj[key] = value;

            skip_whitespace(json, pos);
            if (json[pos] == ',') ++pos;  // Skip ','
        }
        ++pos;  // Skip '}'
        return Value(std::move(obj));
    }

    Value Parser::parse_array(const char* json, size_t& pos) {
        ++pos;  // Skip '['
        std::vector<Value> arr;

        skip_whitespace(json, pos);
        while (json[pos] != ']') {
            arr.push_back(parse_value(json, pos));
            skip_whitespace(json, pos);
            if (json[pos] == ',') ++pos;  // Skip ','
        }
        ++pos;  // Skip ']'
        return Value(std::move(arr));
    }

    Value Parser::parse_string(const char* json, size_t& pos) {
        ++pos;  // Skip opening '"'
        std::ostringstream result;
        while (json[pos] != '"') {
            result << json[pos++];
        }
        ++pos;  // Skip closing '"'
        return Value(result.str());
    }

    Value Parser::parse_number(const char* json, size_t& pos) {
        std::ostringstream result;
        while (std::isdigit(json[pos]) || json[pos] == '.') {
            result << json[pos++];
        }
        return Value(std::stod(result.str()));
    }

    Value Parser::parse_boolean(const char* json, size_t& pos) {
        if (json[pos] == 't') {
            pos += 4;  // Skip "true"
            return Value(true);
        } else {
            pos += 5;  // Skip "false"
            return Value(false);
        }
    }

    Value Parser::parse_null(const char* json, size_t& pos) {
        pos += 4;  // Skip "null"
        return Value();
    }

} // namespace json

