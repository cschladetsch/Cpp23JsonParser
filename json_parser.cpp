#include "json_parser.hpp"
#include <stdexcept>
#include <charconv>

namespace json {

Value Parser::parse(std::string_view json) {
    Parser parser(json);
    return parser.parse_value();
}

Parser::Parser(std::string_view json) : m_json(json), m_pos(0) {}

Value Parser::parse_value() {
    skip_whitespace();
    switch (current()) {
        case 'n': return parse_null();
        case 't': case 'f': return parse_bool();
        case '"': return parse_string();
        case '[': return parse_array();
        case '{': return parse_object();
        default: return parse_number();
    }
}

Value Parser::parse_null() {
    if (m_json.substr(m_pos, 4) == "null") {
        m_pos += 4;
        return nullptr;
    }
    throw std::runtime_error("Invalid null value");
}

Value Parser::parse_bool() {
    if (m_json.substr(m_pos, 4) == "true") {
        m_pos += 4;
        return true;
    }
    if (m_json.substr(m_pos, 5) == "false") {
        m_pos += 5;
        return false;
    }
    throw std::runtime_error("Invalid boolean value");
}

Value Parser::parse_string() {
    std::string result;
    m_pos++; // Skip opening quote
    while (current() != '"') {
        if (current() == '\\') {
            m_pos++;
            switch (current()) {
                case '"': case '\\': case '/': result += current(); break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default: throw std::runtime_error("Invalid escape sequence");
            }
        } else {
            result += current();
        }
        m_pos++;
    }
    m_pos++; // Skip closing quote
    return result;
}

Value Parser::parse_number() {
    const char* begin = m_json.data() + m_pos;
    const char* end = m_json.data() + m_json.size();
    double result;
    auto [ptr, ec] = std::from_chars(begin, end, result);
    if (ec == std::errc()) {
        m_pos += ptr - begin;
        return result;
    }
    throw std::runtime_error("Invalid number");
}

Value Parser::parse_array() {
    Array result;
    m_pos++; // Skip opening bracket
    skip_whitespace();
    if (current() == ']') {
        m_pos++;
        return result;
    }
    while (true) {
        result.push_back(parse_value());
        skip_whitespace();
        if (current() == ']') {
            m_pos++;
            return result;
        }
        if (current() != ',') {
            throw std::runtime_error("Expected ',' or ']' in array");
        }
        m_pos++;
        skip_whitespace();
    }
}

Value Parser::parse_object() {
    Object result;
    m_pos++; // Skip opening brace
    skip_whitespace();
    if (current() == '}') {
        m_pos++;
        return result;
    }
    while (true) {
        auto key = parse_string().get<std::string>();
        skip_whitespace();
        if (current() != ':') {
            throw std::runtime_error("Expected ':' in object");
        }
        m_pos++;
        skip_whitespace();
        result[key] = parse_value();
        skip_whitespace();
        if (current() == '}') {
            m_pos++;
            return result;
        }
        if (current() != ',') {
            throw std::runtime_error("Expected ',' or '}' in object");
        }
        m_pos++;
        skip_whitespace();
    }
}

void Parser::skip_whitespace() {
    while (m_pos < m_json.size() && std::isspace(current())) {
        m_pos++;
    }
}

char Parser::current() const {
    return m_pos < m_json.size() ? m_json[m_pos] : '\0';
}

} // namespace json
